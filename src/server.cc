#include <memory>
#include "server.h"

#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <future>
#include <vector>
#include <fcntl.h>
#include <string.h>

#include <event2/event.h>
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>

namespace lipido {

//ev callback
void ev_http_callback(evhttp_request *req, void *server_instance) {
    WebServer *me = (WebServer*)server_instance;
    me->handleEventCallback(req);
}

static const struct table_entry {
    const char *extension;
    const char *content_type;
} content_type_table[] = {
    { "txt", "text/plain" },
    { "c", "text/plain" },
    { "h", "text/plain" },
    { "html", "text/html" },
    { "htm", "text/htm" },
    { "css", "text/css" },
    { "gif", "image/gif" },
    { "jpg", "image/jpeg" },
    { "jpeg", "image/jpeg" },
    { "png", "image/png" },
    { "pdf", "application/pdf" },
    { "ps", "application/postsript" },
    { NULL, NULL },
};

/* Try to guess a good content-type for 'path' */
static const char *
guess_content_type(const char *path) {
    const char *last_period, *extension;
    const struct table_entry *ent;
    last_period = strrchr(path, '.');
    if (!last_period || strchr(last_period, '/'))
        goto not_found; /* no exension */
    extension = last_period + 1;
    for (ent = &content_type_table[0]; ent->extension; ++ent) {
        if (!evutil_ascii_strcasecmp(ent->extension, extension))
            return ent->content_type;
    }

not_found:
    return "application/misc";
}


WebResponse defaultHandler(WebContext &ctx) {
    WebResponse response;
    response.deliverFile = true;
    response.path = ctx.request.URI;
	response.contentType = guess_content_type(response.path.c_str());
	printf(">>> default handler for %s with type '%s'\n", response.path.c_str(), response.contentType.c_str());
    return response;
}


WebServer::WebServer() {
    m_getHandlers["*"] = defaultHandler;
}

void WebServer::handleEventCallback(evhttp_request *request) {
    printf(">>> handling callback!\n");

    const evhttp_uri *uri = evhttp_request_get_evhttp_uri(request);

    std::string s_uri(evhttp_uridecode(evhttp_uri_get_path(uri), 0, NULL));
    printf(">>> URI is %s\n", s_uri.c_str());

    //prevent shit like /../../../etc/passwd
    if (strstr(s_uri.c_str(), "..")) {
        printf(">>> .. in URI ... we're fucked\n");
        evhttp_send_error(request, HTTP_BADREQUEST, "fuck off");
        return;
    }

    //get handler for URI
    WebServerHandler handler = nullptr;
    if (evhttp_request_get_command(request) == EVHTTP_REQ_GET) {
        handler = m_getHandlers[s_uri];

        //if there's no handler let's try the default handler
        //which will read a file from static and send it
        if (!handler)
            handler = m_getHandlers["*"];

    } else if (evhttp_request_get_command(request) == EVHTTP_REQ_POST) {
        handler = m_postHandlers[s_uri];
    }

    if (!handler) {
        printf(">>> no handler for URI %s found. 404\n", s_uri.c_str());
        evhttp_send_error(request, 404, "your face sucks");
        return;
    }

    WebRequest w_req;
    w_req.URI = s_uri;

    WebSession w_session;
    WebContext w_ctx(*this, w_req, w_session);

    auto resp = handler(w_ctx);

    evbuffer *replbuf = evbuffer_new();
    std::shared_ptr<void> defer(nullptr, [replbuf](void*) {
        printf(">>> freeing reply buffer\n");
        evbuffer_free(replbuf);
    });

    if (!resp.deliverFile) {
        evbuffer_add_printf(replbuf,"%s", resp.body.c_str());
    } else {
        std::string path = "static";
        path += resp.path;
        printf(">>> trying to send file '%s'\n", path.c_str());

        struct stat st;
        if (stat(path.c_str(), &st) != 0) {
            printf(">>> sending 404 ...");
            evhttp_send_error(request, 404, "your face sucks");
            printf("ok\n");
            return;

        }

        size_t size = st.st_size;

        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0) {
            printf(">>> sending 404 ...");
            evhttp_send_error(request, 404, "your face sucks");
            printf("ok\n");
            return;

        }

        evbuffer_add_file(replbuf, fd, 0, size);
    }


    printf(">>> sending response ... ");
    evhttp_add_header(evhttp_request_get_output_headers(request),
                      "Content-Type", resp.contentType.c_str());
    evhttp_send_reply(request, resp.httpCode, "OK", replbuf);
    printf("ok\n");
    return;
}

void WebServer::addGetHandler(std::string URI, WebServerHandler handler) {
    m_getHandlers[URI] = handler;
}

void WebServer::addPostHandler(std::string URI, WebServerHandler handler) {
    m_postHandlers[URI] = handler;
}

void WebServer::run(unsigned short port) {
    printf("server %p listening on port %i ...\n",this, port);

    char buf[512];
    getcwd(buf, sizeof(buf));

    printf("serving static files from %s/static/ ...\n", buf);

    //shamelessly stolen from the libevent http sample
    event_base *base;
    evhttp *http;
    evhttp_bound_socket *handle;

    base = event_base_new();
    http = evhttp_new(base);

    evhttp_set_gencb(http, ev_http_callback, this);

    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", port);

    event_base_dispatch(base);
}
}
