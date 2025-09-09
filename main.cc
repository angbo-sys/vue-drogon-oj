#include <drogon/drogon.h>
using namespace drogon;

int main() {
    //Set HTTP listener address and port
    app().addListener("0.0.0.0", 5555);
    
    // Add a simple route handler for the root path
    app().registerHandler("/", [](const HttpRequestPtr &req,
                                 std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody("<h1>Hello from Drogon!</h1><p>Server is running successfully.</p>");
        callback(resp);
    });

    // Add API endpoints for Vue frontend
    app().registerHandler("/test", [](const HttpRequestPtr &req,
                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // Get JSON data from request
        Json::Value jsonData;
        if (req->getJsonObject()) {
            jsonData = *req->getJsonObject();
        }
        
        // Create response
        Json::Value response;
        response["status"] = "success";
        response["message"] = "POST request received successfully";
        response["received_data"] = jsonData;
        response["timestamp"] = std::to_string(time(nullptr));
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });

    // Add users endpoint
    app().registerHandler("/users", [](const HttpRequestPtr &req,
                                      std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        Json::Value users;
        users["users"] = Json::arrayValue;
        
        // Add some sample users
        Json::Value user1;
        user1["id"] = 1;
        user1["name"] = "张三";
        user1["email"] = "zhangsan@example.com";
        users["users"].append(user1);
        
        Json::Value user2;
        user2["id"] = 2;
        user2["name"] = "李四";
        user2["email"] = "lisi@example.com";
        users["users"].append(user2);
        
        resp->setBody(users.toStyledString());
        callback(resp);
    });

    // Add CORS headers for all responses
    app().registerPostHandlingAdvice([](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    });
    
    // Load config file
    app().loadConfigFile("../config.json");
    // app().loadConfigFile("../config.yaml"); // Commented out due to yaml-cpp dependency
    
    // Run HTTP framework,the method will block in the internal event loop
    app().run();
    return 0;
}
