#include <iostream>
#include <chrono>
#include <thread>
#include "console_log_msg.hpp"

int main() {
    using namespace console_log_msg;

    auto& logger = Program::Logger::getInstance();

    // Alterando o formato do log
    logger.setFormat("[{timestamp}] [{level}] => {message}");

    // Definindo nível mínimo para TRACE
    logger.setLogLevel(Program::LogLevel::TRACE);

    // Exibindo todos os níveis de log
    logger.trace__("Este é um log TRACE");
    logger.debug__("Este é um log DEBUG");
    logger.info__("Este é um log INFO");
    logger.warning__("Este é um log WARNING");
    logger.error__("Este é um log ERROR");
    logger.infot__("Status hppts: ", 200);

    // Usando formatação printf-style
    logger.debugf__("Mensagem formatada com valor: %d", 42);

    // Medindo duração de uma operação simulada
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    auto end = std::chrono::high_resolution_clock::now();
    logger.logWithDuration("Operação simulada", 
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

    // Lidando com exceções
    try {
        throw std::runtime_error("Erro simulado!");
    } catch (const std::exception& e) {
        logger.logException(e);
    }

    // Comentado para evitar encerramento do programa
    // logger.fatal__("Este é um log FATAL");

    return 0;
}


