
 //////////////////

 void start_711()
{
  uchar temp_1;

     temp_1=(P0&0xaa)&((P0&0x55)<<1);

     if(temp_1>0)
	{
	 P0&=~(temp_1>>1);
	 Delay1us();
	 }

     temp_1=(P2&0x55)&((P2&0xaa)>>1);

     if(temp_1>0)
	{
	 P2&=~(temp_1<<1);
	 Delay1us();
	 }


}

//////////////////
uchar read_711(uchar uu)
{
 	 int i;
	 uchar byte;


	 if (uu<4){


		for(i=1;i<=8;i++)
	 	{ 
		   
		   P0|=(0x01<<(uu*2));
 		   Delay1us();
		   byte<<=1;
		   if((P0&(0x01<<(uu*2+1)))>0)  byte+=1;
		   P0&=~(0x01<<(uu*2));
		   Delay1us();					
		}
   	   return(byte);

	 }
	 else{

		for(i=1;i<=8;i++)
	 	{ 
		   
		   P2|=(0x01<<(7-(uu-4)*2));
 		   Delay1us();
		   byte<<=1;
		   if((P2&(0x01<<(7-(uu-4)*2-1)))>0)  byte+=1;
		   P2&=~(0x01<<(7-(uu-4)*2));
		   Delay1us();					
		}
   	   return(byte);

	 }


}
/////////////////////
void stop_711(uchar uu)
{
	 int i;

	 if (uu<4){

	   	for(i=1;i<=3;i++)
	 	{ 
		   P0|=(0x01<<(uu*2));
		   Delay1us();
	       P0&=~(0x01<<(uu*2));
 	       Delay1us();
		   P0|=(0x01<<(uu*2+1));
		}

	 }else{

	   for(i=1;i<=3;i++)
	 	{ 
		   P2|=(0x01<<(7-(uu-4)*2));
		   Delay1us();
	       P2&=~(0x01<<(7-(uu-4)*2));
 	       Delay1us();
		   P2|=(0x01<<(7-uu*2-1));
		}


	 }

}





 ////////////////////////
unsigned long char_uint(unsigned char c1,unsigned char c2,unsigned char c3)
{

     unsigned long temp_int,temp1_int1;
	temp1_int1=c1;
	temp_int=temp1_int1*65536;
	temp1_int1=c2;
	temp_int=temp_int+temp1_int1*256+c3;
	return(temp_int) ;



} 

unsigned long char_uint1(unsigned char c1,unsigned char c2,unsigned char c3)
{
    unsigned long temp_int,temp1_int1;
	temp1_int1=c1;
	temp_int=temp1_int1*65536;
	temp1_int1=c2;
	temp_int=temp_int+temp1_int1*256+c3;
	return(temp_int) ;
} 



		  
 /////////////////
void Init0_711(uchar uu)
{
		  unsigned long temp1_int=0, temp2_int=0;

		  	
			   temp1_int= char_uint1(setting[uu][10],setting[uu][11],setting[uu][12]);
			   temp2_int= char_uint1(setting[uu][13],setting[uu][14],setting[uu][15]);

		  if (temp2_int>temp1_int)
			   {
               //  temp3_int=temp2_int-temp1_int; //
				  zfFlag&=~(0x01<<uu);

				}
		  else

			   {
                // temp3_int=temp1_int-temp2_int; //
				  zfFlag|=(0x01<<uu);
				}



}
 		  
 /////////////////
void Init1_711(uchar uu)
{


 		  ad_f0[uu][0]=ad_f1[uu][0];		                                         	   
		  ad_f0[uu][1]=ad_f1[uu][1];
		  ad_f0[uu][2]=ad_f1[uu][2];	


}

uchar lockdata(void)
{
 	uchar temp_1=0;
	uchar temp_2=0;
	temp_1=~(((P0&0xaa)|((P0&0x55)<<1))|0x55);

    if(temp_1>0)
	{

	 temp_2|=((temp_1&0x02)>>1);
	 temp_2|=((temp_1&0x08)>>2);
	 temp_2|=((temp_1&0x20)>>3);
 	 temp_2|=((temp_1&0x80)>>4);

	 }
	temp_1=(P2&0x55)&((P2&0xaa)>>1);
 	temp_1=~((((P2&0xaa)>>1)|(P2&0x55))|0xaa);

    if(temp_1>0)
	{

	 temp_2|=((temp_1&0x01)<<7);
	 temp_2|=((temp_1&0x04)<<4);
	 temp_2|=((temp_1&0x10)<<1);
 	 temp_2|=((temp_1&0x40)>>2);

	 }	

	return(temp_2) ;

}



 /////////////////
void hx_711(void)

{

	uchar uu;
	uchar temp_1;

  	uchar num1=0,num2=0,num3=0;
	unsigned long decimal,decimal1,decimal2,decimala;	 

	start_711();
	temp_1=lockdata();

	if(temp_1==0){;}
	else{

		for(uu=0;uu<8;uu++){

			if((temp_1&(0x01<<uu))>0){

		  		count_711[uu]++;if(count_711[uu]>10) {count_711[uu]=10;invalid&=~(0x01<<uu); }

			  	wait_sec=1;
	
	   		  	num1=read_711(uu);
			  	num2=read_711(uu);
			  	num3=read_711(uu);
		      	stop_711(uu);
	
			  	wait_sec=0;
	
			  	decimal=char_uint(num1,num2,num3);
	          	if (decimal>10000000)
					decimal=10000000-(16777216-decimal);
			  	else decimal=10000000+decimal;
	

	 		  	//decimal=decimal/2+char_uint(ad_f1[uu][0],ad_f1[uu][1],ad_f1[uu][2])/2; 
	
	
	 		  	ad_f1[uu][0]=decimal/65536;			                                         	  
			  	ad_f1[uu][1]=decimal/256;
			  	ad_f1[uu][2]=decimal%256;
	
	
			  	decimal1=char_uint(ad_f0[uu][0],ad_f0[uu][1],ad_f0[uu][2]) ;

				if(char_uint(setting[uu][13],setting[uu][14],setting[uu][15])>char_uint(setting[uu][10],setting[uu][11],setting[uu][12]))
			         transferParameter=(char_uint(setting[uu][13],setting[uu][14],setting[uu][15])-
									char_uint(setting[uu][10],setting[uu][11],setting[uu][12]))/10+1;
				else
			         transferParameter=(char_uint(setting[uu][10],setting[uu][11],setting[uu][12])-
					 				char_uint(setting[uu][13],setting[uu][14],setting[uu][15]))/10+1;


			  	//transferParameter=5000;
			  
			  
			  	if(setting[uu][1]>0){		  
		  
		  			uint unit;
			    	unit=char_uint(0,setting[uu][2],setting[uu][3]);


		  			if((zfFlag&(0x01<<uu))>0){
		  		
					
					//	unit= char_uint1(0,setting[uu][2],setting[uu][3]);

			  			if(decimal>decimal1){
			  	
			  
					   		decimal2=((decimal-decimal1)*99/transferParameter)*100/unit;		  
							decimala=decimal2%100;
							
							if(decimal2>0||decimala>20)
					  		     setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);      
		
					  	}
			  			else{
			  	
			  
					   		decimal2=((decimal1-decimal)*99/transferParameter)*100/unit;		  
							decimala=decimal2%100;
		
							if(decimala>(100-setting[uu][7]))	 
							    decimal2+=100; 
		
							if(decimala>(setting[uu][6])&&decimala<(100-setting[uu][7]))
					  		     setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);      
		
					  	}					
					

		   			}

		  			else{


				  		if(decimal>decimal1){
					
					   		decimal2=((decimal-decimal1)*99/transferParameter)*100/unit;		  
							decimala=decimal2%100;
		
							if(decimala>(100-setting[uu][7]))	 
							    decimal2+=100; 
		
							if(decimala>(setting[uu][6])&&decimala<(100-setting[uu][7]))
					  		     setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);     
	
						}
				  		else{ 	 
	
	
					   		decimal2=((decimal1-decimal)*99/transferParameter)*100/unit;		  
							decimala=decimal2%100;
		
		
							if(decimal2>0||decimala>20)
					  		     setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);     
	
	
				  		}

					}


	          		averValue=decimal2/100;   
	
				    if(gram_x[uu]==averValue){;}
					else{
					      	gram_x[uu]=averValue;
						  	displayChar|=(0x01<<uu);      
					}
		  
		  		}	
		  		else{	   
	 
			  		if((zfFlag&(0x01<<uu))>0){

				  		if(decimal>decimal1){
				  		
				  
					   		decimal2=((decimal-decimal1)*99)/transferParameter;		  
		
					  		if(decimal2>20)setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);
						
				  		}
				  		else {
	
							 decimal2=((decimal1-decimal)*99)/transferParameter;
							 setting2&=~(0x01<<uu);
	
						}	 
	
			   		}

			  		else{		
	

				  		if(decimal>decimal1){
	 
				     		decimal2=((decimal-decimal1)*99)/transferParameter;	  
							setting2&=~(0x01<<uu);
	
				  		}else {	 
				  		
				     		decimal2=((decimal1-decimal)*99)/transferParameter;
				   	 		if(decimal2>20)setting2|=(0x01<<uu); else setting2&=~(0x01<<uu);    
				  		}
	
					}

          			averValue=decimal2;   

		   
		                                                     //								

	         		if ((averValue+100<100+nx_0)&&(averValue+100>100-nx_0)){
			      	
						nonCommand&=~(0x01<<uu);
				    	if(gram_x[uu]==0){;}
						else{
					      		gram_x[uu]=0;
						  		displayChar|=(0x01<<uu);       //
						}
	
	
				 	} else {
	
			     		nonCommand|=(0x01<<uu);
				   		if((averValue+1000-gram_x[uu])>(1000+nx_1)){
				     		
							if(gram_x[uu]!=(averValue+3)/5*5){
								 
								 gram_x[uu]=(averValue+3)/5*5;
						   		 displayChar|=(0x01<<uu);       //
							}
	
					  	}else if((averValue+1000-gram_x[uu])<(1000-nx_1)){
				     		
							if(gram_x[uu]!=(averValue+2)/5*5){
																	
	                      		gram_x[uu]=(averValue+2)/5*5;
						   		displayChar|=(0x01<<uu);      //
							}
	
					  	}	
					  
				  	}

		  	  	}

			}  	
			  
	   	}  

	}

}
