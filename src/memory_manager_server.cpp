#include <iostream>
#include "my_header.h"  // Include the header where the function is declared

//Server includes
#include <grpcpp/grpcpp.h>
// #include "grpc_test.grpc.pb.h"
// #include "greeter.grpc.pb.h"
#include "greeter.pb.h"


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
// using grpc_test::ServicioDePrueba;
// using grpc_test::MensajeDeEntrada;
// using grpc_test::MensajeDeSalida;
using greeter::Greeter;
using greeter::HelloReply;
using greeter::HelloRequest;


    // namespace memory_manager_server{


    /////////////////
    // Funcion para probar los headers.
    void helloWorldHeaderExample() {
        std::cout << "Esta funcion se declara en memory manager de source y lo del header hace que todo el mundo lo pueda ver." << std::endl;
    }
    /////////////////

    // /////////////////
    // // Servicio de prueba basado en el .proto de prueba grpc_test.proto.
    // // Un servicio puede tener varios metodos.
    // class ServicioDePruebaImpl final : public ServicioDePrueba::Service {
    //     Status Sumar(ServerContext* context, const MensajeDeEntrada* request, MensajeDeSalida* reply) override {
    //         reply->set_mensaje_salida("Server responde a " + request->nombre_entrada());
    //         reply->set_numero_salida(request->numero_entrada() + 1);
    //         return Status::OK;
    //     }
    // };
    // /////////////////
    /////////////////
    // Servicio de prueba basado en el .proto de prueba grpc_test.proto.
    // Un servicio puede tener varios metodos.
    class GreeterImpl final : public Greeter::Service {
        Status SayHello(ServerContext* context, const HelloRequest* request, HelloReply* reply) override {
            reply->set_message("Server responde a " + request->name());
            return Status::OK;
        }
    };
    /////////////////



    // /////////////////
    // void RunServer(const std::string port) {
    //     std::string server_address("0.0.0.0:50051");
    //     // std::string server_address("0.0.0.0:" + port);
    //     ServicioDePruebaImpl service;

    //     ServerBuilder builder;
    //     builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    //     builder.RegisterService(&service);
    //     std::unique_ptr<Server> server(builder.BuildAndStart());
    //     std::cout << "Server listening on " << server_address << std::endl;
    //     server->Wait();
    // }
    // /////////////////

    /////////////////
    void RunServer(const std::string port) {
        std::string server_address("0.0.0.0:50051");
        // std::string server_address("0.0.0.0:" + port);
        GreeterImpl service;

        ServerBuilder builder;
        builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << server_address << std::endl;
        server->Wait();
    }
    /////////////////





// }



