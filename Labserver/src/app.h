#ifndef APP_H
#define APP_H

// ###########################################################################################################################################
// DESENVOLVIMENTO
#define LOG_MODBUS		pdON

// ###########################################################################################################################################
// MODEL WORKTEMP

#define MODEL			"ms10"	// somente 4 chars, por causa so modbus
#define FIRMWARE_VERSION "1.0"	// usar x.y s� com uma casa decimal, pois estamos usando a impress�o de temperatura
								// usar 3 chars por causa do modbus
#define PROGAM_NAME "MSIP" MODEL " firmware v" FIRMWARE_VERSION " [compiled in " __DATE__ " " __TIME__ "]" CMD_TERMINATOR


#define nTHERMOMETER  		 0    	// Quantidade de term�metro que o RH deve gerenciar. O ID modbus do term�metro deve ser sequencial de 1 a quantidade de term�metro
#define EXP_nSLOTS_SAMPES 	 600  	// 600 = 10 minutos. Quantos slots guardaremos para as amostras das temperaturas
#define nBUFFER_SAMPLES_READ 50  	// Quantidade de registradores dos resultado vamos ler por vez. M�ximo 120



// ###########################################################################################################################################
// INCLUDES


// ###########################################################################################################################################
// MODBUS
#define COM_PORT "/dev/ttyAMA0" 	// Consultar /dev/ttySxxx do linux
									// Opensuse com placa pci para serial = "/dev/ttyS4"
									// Raspberry "/dev/ttyAMA0"
									// Para descobrir qual o n� da porta UART fa�a o seguinte:
									// 		Conecte o RASP com um PC via cabo serial.
									//		No PC com um terminal de comnica��o serial abra uma conex�o com RASP a 115200bps 8N1
									//		No RAPS liste as portas tty: ls tty*
									// 		No RAPS na linha de comando envie uma mensagem para cada porta serial: echo ola /dev/ttyXXXX
									//			Fa�a para para todas as portas listadas at� que receba a mensagem no terminal do PC

#define MODBUS_BAUDRATE		B57600 // N�o usar acima de 57600, pois h� erro de recep��o do raspberry.
									// Deve ser algum bug de hardware do rasp porque o baudrate do rasp n�o fica indentico do ARM
									// pois a comunica��o com PC a 115200 funciona bem.
									// Ou a tolerancia de erro no rasp n�o � t�o grande como no PC onde o ARM tem um erro consider�vel
									//	TODO Quando usar o oscilador interno do ARM refazer os testes a sabe com usando oscilador interno do ARM isso se resolve

// ###########################################################################################################################################
// CONTROLE DO SISTEMA

typedef enum {
	cmdNONE = 0,
	cmdGET_INFO,        // RH: Comando para ler os registradores: modelo do RH e vers�o do firmware 	
    cmdSET_TIME,  
    cmdSET_LASER,       
	cmdSET_STATE,
} tCommand;



typedef struct {
	// flash de controle de a��o para acesso ao RH. Esses flags devem ser atualizados via rede
	unsigned exit:1;		// 1 Sinaliza para cair fora do programa
	unsigned getInfo:1;		// Sinaliza para capturar as informa��es do recurso de hardware
	unsigned setSlot:1;
	unsigned int slot;
	unsigned setTimeOn:1;
	unsigned setState:1;
	unsigned int time;
	unsigned int state;
	
	int stsCom;			// Status de comunica��o com RH via modbus
							//		0: Sem comunica��o com o RH. O mesmo n�o est� conectado, ou est� desligado, ou n�o h� dispositivo neste endere�o.
							//		1: O RH recebeu uma fun��o que n�o foi implementada;
							//		2: Foi acessado a um endere�o de registrador inexistente;
							//		3: Foi tentado gravar um valor inv�lido no registrador do RH;
							//		4: Um irrecuper�vel erro ocorreu enquanto o RH estava tentando executar a a��o solicitada;
							//		5: Comunica��o estabelecida com sucesso
	uint rhID;			// Qual o ID do RH no barramento modbus
	string rhModel;		// Modelo do RH
	string rhFirmware;	// Firmware vers�o
} tControl;

typedef enum {readREGS, writeREG, writeREGS} tcmd;


// ###############################################################################
// PROTOTIPOS
int modbus_Init(void);
void modbus_SendCommand( tCommand c) ;
void init_control_tad();
void * modbus_Process(void * params);

#endif
