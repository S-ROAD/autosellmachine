

//#include "reg51.h"
//#include "intrins.h"

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
//int changdu=32;

#define ERROR   0
#define OK      1

//-----------------------------------------------

//sfr IAP_DATA    =   0xC2;     
//sfr IAP_ADDRH   =   0xC3;     
//sfr IAP_ADDRL   =   0xC4;     
//sfr IAP_CMD     =   0xC5;     
//sfr IAP_TRIG    =   0xC6;     
//sfr IAP_CONTR   =   0xC7;     


#define CMD_IDLE    0           
#define CMD_READ    1           
#define CMD_PROGRAM 2           
#define CMD_ERASE   3           


//#define ENABLE_IAP 0x80       
#define ENABLE_IAP 0x81         
//#define ENABLE_IAP  0x82      
//#define ENABLE_IAP 0x83       
//#define ENABLE_IAP 0x84       
//#define ENABLE_IAP 0x85       
//#define ENABLE_IAP 0x86       
//#define ENABLE_IAP 0x87       

//
#define IAP_ADDRESS 0x0000

void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
BYTE readeep(WORD begin_addr, WORD counter, BYTE array[]);
BYTE writeeep(WORD begin_addr, WORD counter, BYTE array[]);




//#define USED_BYTE_QTY_IN_ONE_SECTOR   1
//#define USED_BYTE_QTY_IN_ONE_SECTOR   2
//#define USED_BYTE_QTY_IN_ONE_SECTOR   4
//#define USED_BYTE_QTY_IN_ONE_SECTOR   8
//#define USED_BYTE_QTY_IN_ONE_SECTOR   16
//#define USED_BYTE_QTY_IN_ONE_SECTOR   32
//#define USED_BYTE_QTY_IN_ONE_SECTOR   64
//#define USED_BYTE_QTY_IN_ONE_SECTOR     128
#define USED_BYTE_QTY_IN_ONE_SECTOR   256
//#define USED_BYTE_QTY_IN_ONE_SECTOR   512

//#define DEBUG_Data_Memory_Begin_Sector_addr     0x0400

//BYTE xdata protect_buffer[USED_BYTE_QTY_IN_ONE_SECTOR];



/*----------------------------
¹Ø±ÕIAP
----------------------------*/
void IapIdle()
{
    IAP_CONTR = 0;                 
    IAP_CMD = 0;                   
    IAP_TRIG = 0;                  
    IAP_ADDRH = 0x80;              
    IAP_ADDRL = 0;
}

/*----------------------------
´ÓISP/IAP/EEPROM
----------------------------*/
BYTE IapRead(WORD addr)
{
 xdata   BYTE dat;                 

    IAP_CONTR = ENABLE_IAP;        
    IAP_CMD = CMD_READ;            
    IAP_ADDRL = addr;              
    IAP_ADDRH = addr >> 8;         
    IAP_TRIG = 0x5a;               
    IAP_TRIG = 0xa5;               
    _nop_();                       
    dat = IAP_DATA;                
    IapIdle();                     

    return dat;                    
}

/*----------------------------

----------------------------*/
void IapProgram(WORD addr, BYTE dat)
{
    IAP_CONTR = ENABLE_IAP;        
    IAP_CMD = CMD_PROGRAM;         
    IAP_ADDRL = addr;              
    IAP_ADDRH = addr >> 8;         
    IAP_DATA = dat;                
    IAP_TRIG = 0x5a;               
    IAP_TRIG = 0xa5;               
    _nop_();                       
    IapIdle();
}

/*----------------------------

----------------------------*/
void IapErase(WORD addr)
{
    IAP_CONTR = ENABLE_IAP;         
    IAP_CMD = CMD_ERASE;            
    IAP_ADDRL = addr;               
    IAP_ADDRH = addr >> 8;          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;                
    _nop_();                        
    IapIdle();
}


BYTE writeeep(WORD begin_addr, WORD counter, BYTE array[])
{
  xdata    WORD i = 0;
  xdata    WORD in_sector_begin_addr = 0;
  xdata    WORD sector_addr = 0;



    
    if(counter > USED_BYTE_QTY_IN_ONE_SECTOR)
        return  ERROR;
    in_sector_begin_addr = begin_addr & 0x01ff;
    if((in_sector_begin_addr + counter) > USED_BYTE_QTY_IN_ONE_SECTOR)
        return ERROR;

   
    IapErase(begin_addr);



    for(i=0; i<counter; i++)
    {
        
        IapProgram(begin_addr, array[i]);
        
        if (IapRead(begin_addr) != array[i])
        {
            IapIdle();
            return ERROR;
        }
        begin_addr++;
    }


    IapIdle();
    return  OK;
}

BYTE readeep(WORD begin_addr, WORD counter, BYTE array[])
{
    int i = 0;

    for(i=0; i<counter; i++)
    {

        array[i]=IapRead(begin_addr);
		begin_addr++;
    }
    IapIdle();
    return  OK;
}


