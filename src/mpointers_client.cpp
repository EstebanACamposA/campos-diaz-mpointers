#include <iostream>
#include <grpcpp/grpcpp.h>
// #include "grpc_test.grpc.pb.h"
// #include "greeter.grpc.pb.h"
#include "greeter.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
// using grpc_test::ServicioDePrueba;
// using grpc_test::MensajeDeEntrada;
// using grpc_test::MensajeDeSalida;
using greeter::Greeter;
using greeter::HelloRequest;
using greeter::HelloReply;

// class ClientePrueba {
//     public:
//         ClientePrueba(std::shared_ptr<Channel> channel) : stub_(ServicioDePrueba::NewStub(channel)) {} // Un servicio puede tener varios metodos.
    
//         void FuncionPrueba(std::string& user, int& num) {
//             MensajeDeEntrada request;
//             request.set_nombre_entrada(user);
//             request.set_numero_entrada(num);
//             MensajeDeSalida reply;
//             ClientContext context;
    
//             Status status = stub_->Sumar(&context, request, &reply); // En esta linea se modifica *reply.
//             if (status.ok()) {
//                 std::cout << "Server Response: " << reply.mensaje_salida();
//                 std::cout << ", Server Response: " << reply.numero_salida() << std::endl;
//             } else {
//                 std::cout << "Remote procedure call fallo. Status dio false en FuncionPrueba." << std::endl;
//             }
//         }
    
//         std::unique_ptr<ServicioDePrueba::Stub> stub_;
//     };

    class ClientePrueba {
        public:
            ClientePrueba(std::shared_ptr<Channel> channel) : stub_(Greeter::NewStub(channel)) {} // Un servicio puede tener varios metodos.
        
            void FuncionPrueba(std::string& user, int& num) {
                HelloRequest request;
                request.set_name(user);
                HelloReply reply;
                ClientContext context;
        
                Status status = stub_->SayHello(&context, request, &reply); // En esta linea se modifica *reply.
                if (status.ok()) {
                    std::cout << "Server Response: " << reply.message();
                } else {
                    std::cout << "Remote procedure call fallo. Status dio false en FuncionPrueba." << std::endl;
                }
            }
        
            std::unique_ptr<Greeter::Stub> stub_;
        };
    


    int main() {
        ClientePrueba client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
        std::string nombre = "Emm";
        int num = 787;
        // Must create the variables first to pass them as non const. When the function finishes, these variables might have changed.
        client.FuncionPrueba(nombre, num);
        return 0;
    }