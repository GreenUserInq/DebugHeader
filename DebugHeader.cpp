#include <modbus.h>
#include <iostream>
#include <iomanip> // Для форматированного вывода
#include <cstdlib> // Для exit()

int main() {
    // Параметры Modbus
    const char* port = "/dev/ttyUSB0"; // Порт подключения
    int baudRate = 9600;              // Скорость передачи
    char parity = 'N';                // Чётность: None
    int dataBits = 8;                 // Биты данных
    int stopBits = 1;                 // Стоп-биты
    int slaveID = 1;                  // Адрес устройства
    int regAddress = 0x0101;          // Адрес регистра

    // Создание контекста Modbus
    modbus_t* ctx = modbus_new_rtu(port, baudRate, parity, dataBits, stopBits);
    if (!ctx) {
        std::cerr << "Ошибка создания Modbus контекста\n";
        return -1;
    }

    // Установка Slave ID
    if (modbus_set_slave(ctx, slaveID) == -1) {
        std::cerr << "Ошибка установки Slave ID: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    // Подключение к Modbus
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Ошибка подключения к Modbus: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    // Чтение регистра
    uint16_t value;
    if (modbus_read_registers(ctx, regAddress, 1, &value) == -1) {
        std::cerr << "Ошибка чтения регистра: " << modbus_strerror(errno) << "\n";
        modbus_close(ctx);
        modbus_free(ctx);
        return -1;
    }

    // Вывод значения регистра
    std::cout << "Значение регистра " << std::hex << "0x" << regAddress
        << ": " << std::dec << value << "\n";

    // Освобождение ресурсов
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
