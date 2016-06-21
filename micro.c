// LCD module connections
sbit LCD_RS at RE2_bit;
sbit LCD_EN at RE1_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISE2_bit;
sbit LCD_EN_Direction at TRISE1_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;
// End LCD module connections
int LED = 0;

/* DEFINITIONS */
char readKey();
char mapping [4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
char blueBuffer[4]={47,10,13,'\0'};


void main() {
  int i=20;
  int display = 0;
  char read = 25, last[4]= {'.',10,13,'\0'};
  ADCON1 = 0x06;
  TRISD = 0; //Todo o D como saida
  TRISE = 0; //Todo o E como saida
  TRISC = 0xFF; //Todo o C como entrada
  UART1_Init(9600);


  Lcd_Init();                        // Initialize LCD

  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off

  UART1_Write_Text("Start");
  UART1_Write(10);
  UART1_Write(13);

  while(i--){
   UART1_Write_Text("Start");
   UART1_Write(10);
   UART1_Write(13);
   Delay_ms(1000);

  }


  while(1)
  {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1,1,"lido");
      Lcd_Out(1,5,blueBuffer);
    Lcd_Out(2,1,"Ult");
    Lcd_Out(2,5,last);
    read = readKey();

      if (UART1_Data_Ready())
      {
        blueBuffer[0] = UART1_Read();
      }
    if(read != 25)
    {
      last[0] = read;
      UART1_Write_Text(last);
    }else
    {
      last[0] = '.';
    }

    Delay_ms(15);
  }


}

char readKey(){
  /* Considerations:
   Port-bits order:
   [0-3] Write bits, over vertical lines
         Thus write stars at the first bit and goes up i=3 times
         i starts at 1 so the return expression makes sense
           (Since the first key is the number 1)
   [0-3] Read bits, over horizontal lines
         Thus read starts at 3rd bit and goes up j=3 times
         j starts at 0 so the return expression makes sense
           (Since the first line does not have multipliers)
   write => bit mask for writing on a specific bit ports
   read => read mask for reading on a specific bit port

   @return 25 is default value for NO read
  */
  static int last_state = -1;
  char result=-1;
  int i,j, write =1, read=1, save,
  mask = 0b11110000;
  mask = PORTD & mask;
  save = PORTD;
  for (i=0; i<4; i++)
  {
    read=1;
    PORTD = mask + write;
    for(j=0;j<4;j++)
    {
      if(PORTC&read)
      {
        result = mapping[i][j];
//        result = (result==11)?(0):(result);
        break;
      }
      read = read<<1;
    }
    write = write<<1;
  }
  PORTD = save;
  if(result != -1){
    last_state = result;
    Delay_ms(3);
    return result;
  }
  if (last_state != -1 && result == -1)
  {
    last_state = -1;
  }

  return 25;
}
