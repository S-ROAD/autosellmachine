



  void tx485_init(void)		//115200bps@24.000MHz
{

   
     ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
    P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)




	SCON = 0xD0;	
	AUXR |= 0x40;	
	AUXR &= 0xFE;	
	TMOD &= 0x0F;	
	TL1 = 0xCC;		
	TH1 = 0xFF;		

    TR1 = 1;        
    ES = 1;         
    EA = 1;
    SM2=1;			
}



void send_data(uchar dat,bit jym) 
   {
      while (busy);           
 	  busy = 1;


	  TB8=jym;
	  SBUF = dat;               
	                                                                                                                                                                                                                                                                                                                                             
   }


void send_str(uchar fs[],uchar cd) 
   {
    uchar i;
	for(i=0;i<cd;i++)
    	{send_data(fs[i],0);}
                                                                                                                                                                                                                                                                                                                    
   }

void slave_int(void) interrupt 4 using 1       
   {
	if(RI)	
	{
		 
		RI=0;	        
		accept=SBUF;


		if(SM2){
			 if(accept==0x11){

				  mxFlag2=1;
				  commandChar2=0x01;
			//	  send_data(0x01,0);
				  

			 }

			 if((accept==0x12)&&((leadChar&0x03)==0)&&((leadChar&0x0c)==0)&&(alarmChr==0)){
				  SM2=0;
				  commandChar2=0x02;
				  recLeng=0;

			 }


		}else{
			if (RB8){


				if(accept==0x1f){
	
					  mxFlag2=1;
	 				  SM2=1;

	
				 }
	
	
			}else{

				 kaxinxi[recLeng]=accept;
				 recLeng++;

			}


		}

	}


	if(TI)	          	    
		{
		 TI=0;              
		 busy=0;            
		 }
                                                                                                                                                                                                                                                              
   }
