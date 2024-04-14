#ifndef AMBER_HEADER_MESSAGE_TYPES
#define AMBER_HEADER_MESSAGE_TYPES

namespace amber 
{

namespace http
{
    enum StatusCode
    {
        none                    = 0,
        // 2__ succesful
        ok_200                  = 200,
        created_201             = 201,
        noContent_204           = 204,
        // 3__ redirection
        movedPermanently_301    = 301,
        found_302               = 302,
        temporaryRedirect       = 307,
        permanentRedirect       = 308,
        // 4__ client error
        badRequest_400          = 400,
        unauthorized_401        = 401,
        forbidden_403           = 403,
        notFound                = 404,
    };
}

}

#endif
