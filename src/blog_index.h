/*
 * handler for the blog index (/)
 * (c) Leon Szpilewski, 2012, GPL3
 */

#pragma once
#include "lipido.h"

/*
namespace lipido {
	struct WebResponse;
	struct WebContext;
}
*/

lipido::WebResponse handleIndex(lipido::WebContext &ctx);

