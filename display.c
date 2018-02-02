 void display()
	 {


	 	 if(displayChar>0)
	     {
		   uchar i=0;uchar bbz=0;

		   displayChange|=displayChar;

		   send_data(0x11,1);
		   send_data(displayChar,0);	   


		   for(i=0;i<8;i++){
			  if((displayChar&(0x01<<i))>0)	{
				  gram[i][0]=gram_x[i]/256;
				  gram[i][1]=gram_x[i]%256;

				  send_data(gram[i][0],0);	  ////////
				  send_data(gram[i][1],0);	  /////////////
				  send_price(i);
			  }

		   }

		  send_data(0x1f,1);
		  displayChar=0; 

		 }
	  }
	

