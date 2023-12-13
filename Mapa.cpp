#include <I2C_16Bit.h> // Biblioteca de funções do sensor I2C
#include <Wire.h> // Biblioteca de comunicação I2C
#include <Servo.h> // Biblioteca do servo

#define I2C_ADDR 0x40 // Define o sensor
#define DATA_REGISTER_ADDR 0x5E // Define o registrador de dados do sensor
#define PI 3.141592654 // Define o valor de PI

int coord [26][51]; // Matriz
int i = 0; // Variável de linhas usada para printar zerar a matriz e para imprimí-la
int j = 0; // Variável de colunas usada para printar zerar a matriz e para imprimí-la
int pos = 0; // Variável usada para rotacionar o servo
double ang = 0; // Variável usada para calcular o ângulo em relação ao plano cartesiano do mapa de pontos
int dist = 0; // Variável que armazena a distância, em centímetros, até o osbtáculo medido
int dado = 0; // Variável que armazena o OUTPUT do sensor I2C
float argx = 0; // Variável usada para medir o cosseno da distância aferida
float argy = 0; // Variável usada para medir o seno da distância aferida
int x = 0; // Variável usada para medir o valor no eixo X da distância até o obstáculo
int y = 0; // Variável usada para medir o valor no eixo Y da distância até o obstáculo

void setup(){
	servo_9.attach(9, 500, 2500); // Definindo o servo
	Serial.begin(9600);
	I2C_16Bit_begin(); // Inicializa o sensor
}

void loop(){
    zerador(); // Função para definir o default da matriz
    move(); // Função que faz com que o robô se movimente e desvie de obstáculos
    while (PI == 3.141592654){ // Função usada para que o robô funcione em malha fechada, funcionando até ser desligado 
        for (pos = 0; pos <= 180; pos += 1) { // Sensor girando no sentido horário
            servo_9.write(pos);
            aquisic(); // Função que afere distâncias
            move(); // Função que faz com que o robô se movimente e desvie de obstáculos
            delay(10); // Dessa forma, um ciclo de 180º de sensoriamento demora 1,8 segundos 
        }
        //imprime(); // Função para printar a matriz. Nesse caso, não estou utilizando para não poluir o terminal de programação
        for (pos = 180; pos >= 0; pos -= 1) { // Sensor girando no sentido anti-horário
            servo_9.write(pos);
            aquisic(); // Função que afere distâncias
            move(); // Função que faz com que o robô se movimente e desvie de obstáculos
            delay(10); // Dessa forma, um ciclo de 180º de sensoriamento demora 1,8 segundos 
        }
        //imprime(); // Função para printar a matriz. Nesse caso, não estou utilizando para não poluir o terminal de programação
    }
}

void aquisic(){ // Função que afere distâncias
    uint16_t dado = I2C_16Bit_readFromModule(I2C_ADDR, DATA_REGISTER_ADDR); // Variável que armazena o OUTPUT do sensor I2C e realiza leitura de distâncias
    dist = (dado + 2862)/1348.2; // Função MMQ para aferir a distância
    dist = dist - 4; // Considerando que o sensor está 4cm no interior da base
        if (dist > 0){ // Desconsidera qualquer obstáculo dentro da base
            ang = 180 - pos; // Ângulo entre o sensor e a horizontal definida
            ang = (ang*PI)/180; // Converte o ângulo em graus para radianos
            argx = cos (ang); // Calcula o cosseno do ângulo em radianos
            x = argx * dist; // Calcula a distância no eixo X do obstáculo
            x = 25 + x; // Converte a distância no eixo X do obstáculo para colunas da matriz
            argy = sin (ang); // Calcula o seno do ângulo em radianos
            y = argy * dist; // Calcula a distância no eixo Y do obstáculo
            y = 25 - y; // Converte a distância no eixo Y do obstáculo para linhas da matriz
            if (x >= 0 && x <= 50 && y >= 0 && y <= 25){ // Permite apenas distâncias válidas para a matriz
                coord [y][x] = 1; // Define esse ponto da matriz como 1, o que significa a existência de obstáculos
            }
        }
}

void move(){ // Função que faz com que o robô se movimente e desvie de obstáculos
    if ((coord [25][21] == 1) && (coord [24][21] == 1) && (coord [23][21] == 1) && (coord [25][29] == 1) && (coord [24][29] == 1) && (coord [23][29] == 1) && (coord [21][23] == 1) && (coord [21][24] == 1) && (coord [21][25] == 1) && (coord [21][26] == 1) && (coord [21][27] == 1)){ // Se existem obstáculos na esquerda, na frente e na direita do robô, faz com que ele vá para trás
        // -- COMANDO PARA GIRAR PARA A ESQUERDA -> aumenta a velocidade no motor esquerdo e diminui a velocidade no motor direito
        // -- DELAY DE ACORDO COM o TEMPO QUE O ROBÔ LEVA PARA GIRAR 180º
        zerador(); // Função para definir o default da matriz
        // -- COMANDO PARA MOVIMENTAR PARA FRENTE -> mesma velocidade em ambos os motores
    }
    else if ((coord [25][21] == 1) && (coord [24][21] == 1) && (coord [23][21] == 1)){ // Se existem obstáculos à esquerda do robô, ele vira para a direita
        // -- COMANDO PARA GIRAR PARA A DIREITA -> aumenta a velocidade no motor esquerdo e diminui a velocidade no motor direito
        // -- DELAY DE ACORDO COM o TEMPO QUE O ROBÔ LEVA PARA GIRAR 90º
        zerador(); // Função para definir o default da matriz
        // -- COMANDO PARA MOVIMENTAR PARA FRENTE -> mesma velocidade em ambos os motores
    }
    else if ((coord [25][29] == 1) && (coord [24][29] == 1) && (coord [23][29] == 1)){ // Se existem obstáculos à direita do robô, ele vira para a esquerda
        // -- COMANDO PARA GIRAR PARA A ESQUERDA -> aumenta a velocidade no motor esquerdo e diminui a velocidade no motor direito
        // -- DELAY DE ACORDO COM o TEMPO QUE O ROBÔ LEVA PARA GIRAR 90º
        zerador(); // Função para definir o default da matriz
        // -- COMANDO PARA MOVIMENTAR PARA FRENTE -> mesma velocidade em ambos os motores
    }
    else if ((coord [21][23] == 1) && (coord [21][24] == 1) && (coord [21][25] == 1) && (coord [21][26] == 1) && (coord [21][27] == 1)){ // Se existem obstáculos à frente do robô, ele vira para a esquerda
        // -- COMANDO PARA GIRAR PARA A ESQUERDA -> aumenta a velocidade no motor esquerdo e diminui a velocidade no motor direito
        // -- DELAY DE ACORDO COM o TEMPO QUE O ROBÔ LEVA PARA GIRAR 90º
        zerador(); // Função para definir o default da matriz
        // -- COMANDO PARA MOVIMENTAR PARA FRENTE -> mesma velocidade em ambos os motores
    }
    else{ // Se não existem obstáculos, o robô movimenta-se para frente
        // -- COMANDO PARA MOVIMENTAR PARA FRENTE -> mesma velocidade em ambos os motores
    }
}

void imprime(){ // Função para printar a matriz
    Serial.println();
    Serial.println();
    Serial.println();
    for (i = 0; i < 26; i++){
        for (j = 0; j < 51; j++){
            Serial.print (coord [i][j]);
        }
        Serial.println();
    }
}

void zerador(){ // Função para definir o default da matriz
    for (i = 0; i < 26; i++){ // For usado para zerar todos os valores da matriz. Para esse sistema, 1 significa existência de obstáculos e 0 significa ausência de obstáculos
        for (j = 0; j < 51; j++){
            coord [i][j] = 0;
        }
    }
    coord [25][25] = 2; // Define o centro da matriz como 2. Assim, o 2 significa a origem do plano cartesiano que mapeia os pontos
}