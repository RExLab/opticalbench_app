#ifndef APP_H
#define APP_H

// ###########################################################################################################################################
// DESENVOLVIMENTO
#define LOG_MODBUS		pdON

// ###########################################################################################################################################
// MODEL WORKTEMP

#define MODEL			"ms10"	// somente 4 chars, por causa so modbus
#define FIRMWARE_VERSION "1.0"	// usar x.y só com uma casa decimal, pois estamos usando a impressão de temperatura
								// usar 3 chars por causa do modbus
#define PROGAM_NAME "MSIP" MODEL " firmware v" FIRMWARE_VERSION " [compiled in " __DATE__ " " __TIME__ "]" CMD_TERMINATOR


#define nTHERMOMETER  		 0    	// Quantidade de termômetro que o RH deve gerenciar. O ID modbus do termômetro deve ser sequencial de 1 a quantidade de termômetro
#define EXP_nSLOTS_SAMPES 	 600  	// 600 = 10 minutos. Quantos slots guardaremos para as amostras das temperaturas
#define nBUFFER_SAMPLES_READ 50  	// Quantidade de registradores dos resultado vamos ler por vez. Máximo 120



// ###########################################################################################################################################
// INCLUDES


// ###########################################################################################################################################
// MODBUS
#define COM_PORT "/dev/ttyAMA0" 	// Consultar /dev/ttySxxx do linux
									// Opensuse com placa pci para serial = "/dev/ttyS4"
									// Raspberry "/dev/ttyAMA0"
									// Para descobrir qual o nó da porta UART faça o seguinte:
									// 		Conecte o RASP com um PC via cabo serial.
									//		No PC com um terminal de comnicação serial abra uma conexão com RASP a 115200bps 8N1
									//		No RAPS liste as portas tty: ls tty*
									// 		No RAPS na linha de comando envie uma mensagem para cada porta serial: echo ola /dev/ttyXXXX
									//			Faça para para todas as portas listadas até que receba a mensagem no terminal do PC

#define MODBUS_BAUDRATE		B57600 // Não usar acima de 57600, pois há erro de recepção do raspberry.
									// Deve ser algum bug de hardware do rasp porque o baudrate do rasp não fica indentico do ARM
									// pois a comunicação com PC a 115200 funciona bem.
									// Ou a tolerancia de erro no rasp não é tão grande como no PC onde o ARM tem um erro considerável
									//	TODO Quando usar o oscilador interno do ARM refazer os testes a sabe com usando oscilador interno do ARM isso se resolve

// ###########################################################################################################################################
// CONTROLE DO SISTEMA

typedef enum {
	cmdNONE = 0,
	cmdGET_INFO,        // RH: Comando para ler os registradores: modelo do RH e versão do firmware 	
    cmdSET_TIME,  
    cmdSET_LASER,       
	cmdSET_STATE,
} tCommand;



typedef struct {
	// flash de controle de ação para acesso ao RH. Esses flags devem ser atualizados via rede
	unsigned exit:1;		// 1 Sinaliza para cair fora do programa
	unsigned getInfo:1;		// Sinaliza para capturar as informações do recurso de hardware
	unsigned setSlot:1;
	unsigned int slot;
	unsigned setTimeOn:1;
	unsigned setState:1;
	unsigned int time;
	unsigned int state;
	
	int stsCom;			// Status de comunicação com RH via modbus
							//		0: Sem comunicação com o RH. O mesmo não está conectado, ou está desligado, ou não há dispositivo neste endereço.
							//		1: O RH recebeu uma função que não foi implementada;
							//		2: Foi acessado a um endereço de registrador inexistente;
							//		3: Foi tentado gravar um valor inválido no registrador do RH;
							//		4: Um irrecuperável erro ocorreu enquanto o RH estava tentando executar a ação solicitada;
							//		5: Comunicação estabelecida com sucesso
	uint rhID;			// Qual o ID do RH no barramento modbus
	string rhModel;		// Modelo do RH
	string rhFirmware;	// Firmware versão
} tControl;

typedef enum {readREGS, writeREG, writeREGS} tcmd;


// ###############################################################################
// PROTOTIPOS
int modbus_Init(void);
void modbus_SendCommand( tCommand c) ;
void init_control_tad();
void * modbus_Process(void * params);

#endif
