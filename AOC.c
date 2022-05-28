#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char memoria[154],ro0,ro1,ir,e,l,g;
unsigned int mbr,pc=0,mar,imm,reg[8];
char *param1, *param2, *param3;

void split(char str[100], char delim[]){
	int init_size = strlen(str),i=0;
	char *ptr = strtok(str, delim);

	while(ptr != NULL)
	{
    if(i == 0){
      param1 = ptr;
    }
    else if(i == 1){
      param2 = ptr;
    }
    else if(i == 2){
      param3 = ptr;
    }
    i++;
		ptr = strtok(NULL, delim);
	}
}

//Salva a instrucao do mbr na Memoria
void salvaMemoria(){
  memoria[mar++] = mbr >> 24;
  memoria[mar++] = (mbr >> 16)&0x00ff;
  memoria[mar++] = (mbr >> 8)&0x0000ff;
  memoria[mar++] = mbr & 0x000000ff;
}

//Retorna registrador
int returnReg(char *re){
  int reg = 0;
  if (strcmp(re,"r1")==0)
    reg = 1;
  else if (strcmp(re,"r2")==0)
    reg = 2;
  else if (strcmp(re,"r3")==0)
    reg = 3;
  else if (strcmp(re,"r4")==0)
    reg = 4;
  else if (strcmp(re,"r5")==0)
    reg = 5;
  else if (strcmp(re,"r6")==0)
    reg = 6;
  else if (strcmp(re,"r7")==0)
    reg = 7;
  return reg;
}

//Decodifica Instrucoes do Arquivo
void preencherMemoria(char comando[100]){
  char *pos, *tipo, *comand, *reg, *dado; 
  int pos2, dado2;
  split(comando, ";");
  pos = param1;
  tipo = param2;
  sscanf(pos, "%x",&pos2);
  split(param3," ");
  comand =  param1;
  if(!(strcmp(tipo,"d")==0)){
    char re;
    reg = param2;
    dado = param3;
    split(reg, ",");
    reg = param1;
    sscanf(dado, "%x",&dado2);
    sscanf(dado, "%s",&*dado);
  }
  mar = pos2;
  if(strcmp(tipo,"d")==0){
    sscanf(comand, "%x", &mbr);
    salvaMemoria();
  }else if(strcmp(comand,"hlt")==0){
    mbr = 0x00;
    salvaMemoria();
  }else if(strcmp(comand,"nop")==0){
    mbr = 0x01000000;
    salvaMemoria();
  }else if(strcmp(comand,"add")==0 || strcmp(comand,"sub")==0 || strcmp(comand,"mul")==0 || strcmp(comand,"div")==0 || strcmp(comand,"cmp")==0 || strcmp(comand,"movr")==0 || strcmp(comand,"and")==0 || strcmp(comand,"or")==0 || strcmp(comand,"xor")==0){
    if(strcmp(comand,"add")==0){
      mbr = 0x02;
    }else if(strcmp(comand,"sub")==0){
      mbr = 0x03;
    }else if(strcmp(comand,"mul")==0){
      mbr = 0x04;
    }else if(strcmp(comand,"div")==0){
      mbr = 0x05;
    }else if(strcmp(comand,"cmp")==0){
      mbr = 0x06;
    }else if(strcmp(comand,"movr")==0){
      mbr = 0x07;
    }else if(strcmp(comand,"and")==0){
      mbr = 0x08;
    }else if(strcmp(comand,"or")==0){
      mbr = 0x09;
    }else if(strcmp(comand,"xor")==0){
      mbr = 0x0A;
    }
    int re = returnReg(reg);
    int re2 = returnReg(dado);
    mbr = mbr << 3;
    mbr = (mbr | re);
    mbr = mbr << 3;
    mbr = (mbr | re2);
    mbr = mbr << 18;
    salvaMemoria();
  }else if(strcmp(comand,"not")==0){
    int re = strtol(&reg[1], NULL, 10);
    mbr = 0x0B;
    mbr = mbr << 3;
    mbr = (mbr | re);
    mbr = mbr << 21;
    salvaMemoria();
  }else if(strcmp(comand,"je")==0 || strcmp(comand,"jne")==0 || strcmp(comand,"jl")==0 || strcmp(comand,"jle")==0 || strcmp(comand,"jg")==0 || strcmp(comand,"jge")==0 || strcmp(comand,"jmp")==0){
    if(strcmp(comand,"je")==0){
      mbr = 0x0C;
    }else if(strcmp(comand,"jne")==0){
      mbr = 0x0D;
    }else if(strcmp(comand,"jl")==0){
      mbr = 0x0E;
    }else if(strcmp(comand,"jle")==0){
      mbr = 0x0F;
    }else if(strcmp(comand,"jg")==0){
      mbr = 0x10;
    }else if(strcmp(comand,"jge")==0){
      mbr = 0x11;
    }else if(strcmp(comand,"jmp")==0){
      mbr = 0x12;
    }
    int mem;
    sscanf(reg,"%x",&mem);
    mbr = mbr << 24;
    mbr = (mbr | mem);
    salvaMemoria();
  }else if(strcmp(comand,"ld")==0 || strcmp(comand,"st")==0){
    if(strcmp(comand,"ld")==0){
      mbr = 0x13;
    }else if(strcmp(comand,"st")==0){
      mbr = 0x14;
    }
    int re = strtol(&reg[1], NULL, 10);
    mbr = mbr << 3;
    mbr = (mbr | re);
    mbr = mbr << 21;
    mbr = (mbr | dado2);
    salvaMemoria();
  }else if(strcmp(comand,"movi")==0 || strcmp(comand,"addi")==0 || strcmp(comand,"subi")==0 || strcmp(comand,"muli")==0 || strcmp(comand,"divi")==0 || strcmp(comand,"lsh")==0 || strcmp(comand,"rsh")==0){
    if(strcmp(comand,"movi")==0){
      mbr = 0x15;
    }else if(strcmp(comand,"addi")==0){
      mbr = 0x16;
    }else if(strcmp(comand,"subi")==0){
      mbr = 0x17;
    }else if(strcmp(comand,"muli")==0){
      mbr = 0x18;
    }else if(strcmp(comand,"divi")==0){
      mbr = 0x19;
    }else if(strcmp(comand,"lsh")==0){
      mbr = 0x1A;
    }else if(strcmp(comand,"rsh")==0){
      mbr = 0x1B;
    }
    int re = strtol(&reg[1], NULL, 10);
    mbr = mbr << 3;
    mbr = (mbr | re);
    mbr = mbr << 21;
    mbr = (mbr | dado2);
    salvaMemoria();
  }
}

//Leitura do arquivo a ser executado
void lerArquivo(char* arquivo){
  FILE *arq;
  char Linha[100];
  char *result, tip[1], cmd[10], re[2];
  int i, pos, mem;
  // Abre um arquivo TEXTO para LEITURA
  arq = fopen(arquivo, "rt");
  if (arq == NULL)  // Se houve erro na abertura
  {
    printf("Problemas na abertura do arquivo\n");
    return;
  }
  i = 1;
  while (!feof(arq))
  {
      result = fgets(Linha, 100, arq); 
      if (result){ // Se foi possivel ler
        preencherMemoria(Linha); // Chama funcao de preencher memoria passando a linha de instrucao
      }
      i++;
  }
  fclose(arq);
}

//Busca Instrucao na Memoria
void busca(){
  mbr = 0;
  mbr = memoria[mar++];
  mbr = mbr << 8;
  mbr = (mbr | memoria[mar++]);
  mbr = mbr << 8;
  mbr = (mbr | memoria[mar++]);
  mbr = mbr << 8;
  mbr = (mbr | memoria[mar++]);
}

//Imprimir Dados na Tela
void tela(){
  printf("\n\nCPU:\n");
  printf("\nR0:  0x%08x | R1:  0x%08x  | R2:  0x%08x | R3: 0x%08x",reg[0],reg[1],reg[2],reg[3]);
  printf("\nR4:  0x%08x | R5:  0x%08x  | R6:  0x%08x | R7: 0x%08x",reg[4],reg[5],reg[6],reg[7]);
  printf("\nMBR: 0x%08x | MAR: 0x%08x  | IMM: 0x%08x | PC: 0x%08x",mbr,mar,imm,pc);
  printf("\nIR:  0x%02x       | RO0: 0x%02x        | RO1: 0x%02x       |",ir,ro0,ro1);
  printf("\nE:   0x%x        | L:   0x%x         | G:   0x%x        |",e,l,g);
  printf("\n\nMemoria:\n");
  int i;
  for(i=1; i<=154; i++){
    printf("%02x: %02x  ",i-1,memoria[i-1]);
    if((i%7==0))
      printf("\n");
  }
}

//Main
int main(){
  //Arquivo com instrucoes a ser executado
  lerArquivo("comandos.txt");
  //Ciclo da maquina
  while(pc < sizeof(memoria)){
  	mar = pc; // Proxima instrucao a ser executada
    busca();  // Busca instrucao
    //Decodifica/Executa Instrucoes
    ir = mbr >> 24; // Pega Opcode da instrucao
    if(ir == 0 || ir == 1){
      //Nop / Halt
      if(ir == 1){
        printf("\nNOP");
        pc+=4;
      }else{
        printf("\nHalt");
        pc = sizeof(memoria);
      }
    }else if(ir >= 2 && ir <=11){
      ro0 = (mbr & 0x00e00000) >> 21;
      ro1 = (mbr >> 18) & 0x0007;
      if(ir == 2){
        //Add
        printf("\nAdd");
        reg[ro0] = reg[ro0] + reg[ro1];
        pc+=4;
      }else if (ir == 3){
        //sub
        printf("\nSub");
        reg[ro0] = reg[ro0] - reg[ro1];
        pc+=4;
      }else if (ir == 4){
        //Mul
        printf("\nMul");
        reg[ro0] = reg[ro0] * reg[ro1];
        pc+=4;
      }else if (ir == 5){
        //div
        printf("\nDiv");
        reg[ro0] = reg[ro0] / reg[ro1];
        pc+=4;
      }else if (ir == 6){
        //cmp
        printf("\nCmp");
        if(reg[ro0] == reg[ro1])
          e = 1;
        else
          e = 0;
        if(reg[ro0] < reg[ro1])
          l = 1;
        else
          l = 0;
        if(reg[ro0] > reg[ro1])
          g = 1;
        else
          g = 0;
        pc+=4;
      }else if (ir == 7){
        //movr
        printf("\nMovr");
        reg[ro0] = reg[ro1];
        pc+=4;
      }else if (ir == 8){
        //and
        printf("\nAnd");
        reg[ro0] = reg[ro0] & reg[ro1];
        pc+=4;
      }else if (ir == 9){
        //or
        printf("\nOr");
        reg[ro0] = reg[ro0] | reg[ro1];
        pc+=4;
      }else if (ir == 10){
        //xor
        printf("\nXor");
        reg[ro0] = reg[ro0] ^ reg[ro1];
        pc+=4;
      }else if (ir == 11){
        //not
        printf("\nNot");
        reg[ro0] = !reg[ro0];
        pc+=4;
      }
    }else if(ir >= 12 & ir <= 18){
      mar = mbr&0x1ffff;
      if(ir == 12){
        //je
        if(e == 1)
          pc = mar;
        else 
          pc+=4;
        printf("\nJe");
      }else if(ir == 13){
        //jne
        if(e == 0)
          pc = mar;
        else 
          pc+=4;
        printf("\nJne");
      }else if(ir == 14){
        //jl
        if(l == 1)
          pc = mar;
        else 
          pc+=4;
        printf("\nJl");
      }else if(ir == 15){
        //jle
        if(e == 1 || l == 1)
          pc = mar;
        else 
          pc+=4;
        printf("\nJle");
      }else if(ir == 16){
        //jg
        if(g == 1)
          pc = mar;
        else 
          pc+=4;
        printf("\nJg");
      }else if(ir == 17){
        //jge
        if(e == 1 || g == 1)
          pc = mar;
        else 
          pc+=4;
        printf("\nJge");
      }else if(ir == 18){
        //jmp
        pc = mar;
        printf("\nJmp");
      }
    }else if(ir>=19 && ir<=20){
      ro0 = (mbr & 0x00e00000) >> 21;
      if(ir ==  19){
        //Load
        mar = mbr&0x1ffff;
        busca();
        printf("\nLoad");
        reg[ro0] = mbr;
        pc+=4;
      }else{
        //Stor
        mar = mbr&0x1ffff;
        printf("\nSTOR");
        mbr = reg[ro0];
        salvaMemoria();
        pc+=4;
      }
    }else if(ir>=21 && ir <= 27){
      //Addi
      printf("\nAddi");
      imm = mbr&0x1ffff;
      ro0 = (mbr & 0x000e00000) >> 21;
      if(ir == 21){
        //movi
        reg[ro0] = imm;
        pc +=4;
      }else if(ir == 22){
        //addi
        reg[ro0] += imm;
        pc+=4;
      }else if(ir == 23){
        //subi
        reg[ro0] -= imm;
        pc+=4;
      }else if(ir == 24){
        //muli
        reg[ro0] = reg[ro0] * imm;
        pc+=4;
      }else if(ir == 25){
        //divi
        reg[ro0] = reg[ro0] / imm;
        pc+=4;
      }else if(ir == 26){
        //lsh
        reg[ro0] = reg[ro0] << imm;
        pc+=4;
      }else if(ir == 27){
        //rsh
        reg[ro0] = reg[ro0] >> imm;
        pc+=4;
      }
    }
    tela();
    system("PAUSE");
    system("clear");
  }
  return 0;
}

