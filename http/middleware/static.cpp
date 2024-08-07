#include "static.h"
#include "core/config.h"
#include "log/log.h"
#include <fstream>

auto amber::http::middleware::serveStatic(Request &req, Response &res) -> bool
{
        if (req.getMethod() != Method::get)
                return true;
        std::string path;
        path.insert(0, "/").insert(0, amber::config::publicRootPath()).append(req.getPath());
        std::stringstream outputBuffer;
        std::ifstream file;
        file.open(path, std::iostream::binary);

        auto reqPath = req.getPath();
        std::string_view ext;
        if (auto pos = reqPath.find_last_of('.'); pos != std::string::npos)
                ext = reqPath.substr(pos);

        if (!file.is_open() || !file.good() || ext.empty()) {
                LOG_TRACE("no static file found: " << req.getPath());
                res.setStatus(http::StatusCode::notFound_404);
                return true;
        }
        outputBuffer << file.rdbuf();
        res.setBody(outputBuffer.str());
        res.setHeader("Content-Type", amber::http::fileExtToMimeType(ext));
        res.setHeader("Connection", "keep-alive");
        res.setStatus(amber::http::ok_200);
        LOG_TRACE("served static file: " << req.getPath());
        return false;
}
