#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura
typedef struct {
    struct {
        uint8_t id[3];
        uint8_t size[4];
    } header;
    uint16_t left;
    uint16_t right;
    uint8_t bouncesL;
    uint8_t bouncesR;
    uint8_t feedBackLL;
    uint8_t feedBackLLR;
    uint8_t feedBackRR;
    uint8_t feedBackRL;
    uint8_t premixLR;
    uint8_t premixRL;
} ID3v2RVBRFrameType;

// Función para convertir una cadena hexadecimal a un número
uint16_t hexStrToUInt16(const char *str) {
    uint16_t value;
    if (sscanf(str, "%hx", &value) == 1) {
        return value;
    } else {
        return 0; // Retorna 0 si la conversión falla
    }
}

uint8_t hexStrToUInt8(const char *str) {
    uint8_t value;
    if (sscanf(str, "%hhx", &value) == 1) {
        return value;
    } else {
        return 0; // Retorna 0 si la conversión falla
    }
}

// Función para solicitar datos al usuario
void solicitarDatos(ID3v2RVBRFrameType *frame) {
    char buffer[10]; // Para almacenar la entrada del usuario

    printf("Introduce los valores para el 'Reverb' en hexadecimal.\n");

    // Reverb left (ms)
    printf("Reverb left (ms): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->left = hexStrToUInt16(buffer);

    // Reverb right (ms)
    printf("Reverb right (ms): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->right = hexStrToUInt16(buffer);

    // Reverb bounces, left
    printf("Reverb bounces, left (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->bouncesL = hexStrToUInt8(buffer);

    // Reverb bounces, right
    printf("Reverb bounces, right (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->bouncesR = hexStrToUInt8(buffer);

    // Reverb feedback, left to left
    printf("Reverb feedback, left to left (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->feedBackLL = hexStrToUInt8(buffer);

    // Reverb feedback, left to right
    printf("Reverb feedback, left to right (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->feedBackLLR = hexStrToUInt8(buffer);

    // Reverb feedback, right to right
    printf("Reverb feedback, right to right (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->feedBackRR = hexStrToUInt8(buffer);

    // Reverb feedback, right to left
    printf("Reverb feedback, right to left (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->feedBackRL = hexStrToUInt8(buffer);

    // Premix left to right
    printf("Premix left to right (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->premixLR = hexStrToUInt8(buffer);

    // Premix right to left
    printf("Premix right to left (0-255): ");
    fgets(buffer, sizeof(buffer), stdin);
    frame->premixRL = hexStrToUInt8(buffer);
}

int main() {
    ID3v2RVBRFrameType frame;

    // Llamamos a la función para solicitar los datos
    solicitarDatos(&frame);

    // Mostrar los valores introducidos para confirmar
    printf("\nValores introducidos:\n");
    printf("Reverb left (ms): %04X\n", frame.left);
    printf("Reverb right (ms): %04X\n", frame.right);
    printf("Reverb bounces, left: %02X\n", frame.bouncesL);
    printf("Reverb bounces, right: %02X\n", frame.bouncesR);
    printf("Reverb feedback, left to left: %02X\n", frame.feedBackLL);
    printf("Reverb feedback, left to right: %02X\n", frame.feedBackLLR);
    printf("Reverb feedback, right to right: %02X\n", frame.feedBackRR);
    printf("Reverb feedback, right to left: %02X\n", frame.feedBackRL);
    printf("Premix left to right: %02X\n", frame.premixLR);
    printf("Premix right to left: %02X\n", frame.premixRL);

    return 0;
}
