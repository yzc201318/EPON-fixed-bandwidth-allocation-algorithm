#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<string.h>
#define link_rate 1e3   //��·����1e9bits/s,��1e3bits/us 
#define packet_size 512 //����Ϊ64bytes,��512bits 
#define vacation_time (1*packet_size/link_rate) //�û���Ϣʱ��
#define service_rate (link_rate/packet_size) //��������
#define M 2  //�û�ÿ��������������
long long int mod=1;
long long int randn;
long long int buffer_num=0;
int sent_num=0; //ÿ�������û�����İ���
long long int nonzero_cycle=0;  //æ�ڲ�Ϊ0������ 
long long int total_sent_num=0;
long long int total_arr_num=0; 
long long int total_arr_inbusy=0; //busy period�ڼ䵽��İ��� 
long long int total_vacanum=0;
long long int total_numingate=0; //�����ܵİ��� 
long long int total_numoutgate=0; //�����ܵİ���
long long int total_queuelength=0; //�ܶӳ� 
long long int num_outgate[50000]={0}; //��¼��������ֲ� 
long long int num_ingate[50000]={0};  //��¼���ڰ����ֲ� 
long long int num_sent[50000]={0};  //��¼ÿ�ι���ʱ�̷Ž����ڵİ������ֲ� 
long long int numout_vacaend[50000]={0}; //��¼vacation��������������ֲ� 
long long int vacation_num[50000]={0}; //��¼ÿ����������vacation�����ֲ� 
int cycle_num; //����������
int m; //��¼������ʱ���������ڰ��� 
int n; //��¼������ʱ�������������
int k; //��¼������ʱ�����Ķӳ� 
int va=0; //��¼����ȥʱ������vacation���� 
long long int arr_cycle[2000000]={0}; //������������� 
double system_time=0; //ϵͳʱ�� 
double cycle_begin_time=0; //���ڿ�ʼʱ��
double cycle_end_time=0; //���ڽ���ʱ�� 
double transtime=0;
double arr_time[2000000]={0}; //��¼ÿ�����ĵ���ʱ�� 
double depart_time[2000000]={0};  //��¼ÿ�������뿪ʱ��
double total_delay=0;
double total_busyperiod=0;
double ave_busyperiod=0; //ƽ��æ�� 
double total_delayingate=0; //�������ڵ���ʱ��
double total_cycletime=0; 
FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6,*fp7,*fp8,*fp9;

 
 /********ָ���ֲ�********/
void init()
{
    int i;
    mod=1;
    randn = time(NULL);
    for(i=0;i<35;i++) mod *= 2;
    mod = mod - 31;
}

long long int ran(long long int mod)
{
	randn = 5*5*5*5*5* randn% mod;
    return randn ;
}

double exprand(double para)             
{
	double q;
	q=-1/para*log((double)ran(mod)/(mod-1));
    return q;
}
/********ָ���ֲ�********/


void run(double offerload){
    int i;
	cycle_begin_time=cycle_end_time;
	if(buffer_num<=M){
	   sent_num=buffer_num;
    }
    else sent_num=M;
    num_sent[sent_num]++; 
    n=buffer_num-sent_num;
	numout_vacaend[n]++;
	transtime=sent_num*packet_size/link_rate;
/*	printf("����%d\n",cycle_num);
	fprintf(fp4,"����%d\n",cycle_num);
	printf("���ڿ�ʼʱ��%f\n",cycle_begin_time);
	fprintf(fp4,"���ڿ�ʼʱ��%f\n",cycle_begin_time);
	printf("���ڿ�ʼ���ڸ���%d\n",sent_num);
	fprintf(fp4,"���ڿ�ʼ���ڸ���%d\n",sent_num);
*/
	cycle_end_time=cycle_begin_time+transtime+vacation_time;
	while(system_time<cycle_begin_time+transtime){
	   system_time+=exprand(offerload*service_rate);
	   arr_time[total_arr_num%2000000]=system_time;
	   arr_cycle[total_arr_num%2000000]=cycle_num;
	   m=int((cycle_begin_time+transtime-system_time)*link_rate/packet_size);
	   num_ingate[m]++; 
	   total_numingate+=m;
	   n=buffer_num-sent_num;
	   num_outgate[n]++;
	   total_numoutgate+=n;
	   buffer_num++;
	   total_arr_num++;
	   total_arr_inbusy++; 
/*	   if(system_time<cycle_begin_time+transtime){
	   printf("time%f\n",system_time);
	   fprintf(fp4,"time%f\n",system_time);
	   fprintf(fp5,"%f\n",system_time);
	   printf("���ڸ���%d\n",m);
	   fprintf(fp4,"����δ�������%d\n",m);
	   fprintf(fp3,"%d\n",m);
   	}
*/
}
   if(sent_num>0){
   	 buffer_num--;
     total_arr_num--;
     total_arr_inbusy--;
     num_ingate[m]--;
     total_numingate-=m;
     num_outgate[n]--;
     total_numoutgate-=n;
     total_queuelength=total_queuelength-m-n;
   	}
   system_time=cycle_begin_time+transtime;
   while(system_time<cycle_end_time){
   	   system_time+=exprand(offerload*service_rate);
	   arr_time[total_arr_num%2000000]=system_time;
	   arr_cycle[total_arr_num%2000000]=cycle_num;
	   num_ingate[0]++;  
	   n=buffer_num-sent_num;
	   num_outgate[n]++;
	   total_numoutgate+=n;
       total_queuelength=total_queuelength+n;
	   buffer_num++;
	   total_arr_num++;
  	}
   num_outgate[n]--;
   total_numoutgate-=n;
   num_ingate[0]--;
   total_queuelength=total_queuelength-n;
   buffer_num--;
   total_arr_num--;
   for(i=0;i<sent_num;i++){
  	    depart_time[(total_sent_num+i)%2000000]=cycle_begin_time+i*packet_size/link_rate;
 	    total_delay=total_delay+depart_time[(total_sent_num+i)%2000000]-arr_time[(total_sent_num+i)%2000000];
 	    va=cycle_num-arr_cycle[(total_sent_num+i)%2000000]-1;
 	    vacation_num[va]++;
		total_vacanum=total_vacanum+va;
		total_delayingate+=i*packet_size/link_rate; 
    }  
	buffer_num-=sent_num;
	total_sent_num+=sent_num;          
    total_busyperiod+=transtime;
    total_cycletime=total_cycletime+transtime+vacation_time;
   	system_time=cycle_end_time;
}


main(){
	
	
	double offerload,ave_delay,ave_vacanum;
	double ave_numingate,ave_numoutgate,ave_queuelength,ave_sentnumincycle,ave_delayingate,ave_cycletime;
	int j;
//	fp1=fopen("average_delay.txt","w");
	fp2=fopen("���ڰ�����Ϊ0����.txt","w");
	fp3=fopen("���ڰ�����Ϊ1����.txt","w");
//	fp4=fopen("���ڰ�����Ϊ2����.txt","w");
//	fp5=fopen("���ڰ�����Ϊ3����.txt","w");
//	fp6=fopen("��������ֲ�.txt","w");
//	fp7=fopen("vacation�����ֲ�.txt","w");
//	fp8=fopen("ƽ�����ڵȴ�����.txt","w");
//	fp9=fopen("average_sentnumincycle.txt","w");
	for(offerload=0.1;offerload<0.61;offerload+=0.05){
		init();
		total_delay=0;
		total_busyperiod=0;
		total_queuelength=0;
		total_delayingate=0;
	    buffer_num=0;
	    sent_num=0;
	    nonzero_cycle=0;
	    total_sent_num=0;
	    total_arr_num=0;
	    total_arr_inbusy=0;
	    system_time=0;
	    total_vacanum=0;
	    total_numingate=0;
	    total_numoutgate=0;
	    total_cycletime=0;
	    cycle_begin_time=0;
	    cycle_end_time=0;
	    memset(arr_time,0, sizeof(arr_time));
	    memset(depart_time,0,sizeof(depart_time));
	    memset(arr_cycle,0, sizeof(arr_cycle));
	    memset(num_ingate,0,sizeof(num_ingate));
	    memset(num_outgate,0,sizeof(num_outgate));
	    memset(numout_vacaend,0,sizeof(numout_vacaend));
	    memset(vacation_num,0,sizeof(vacation_num));
	    memset(num_sent,0,sizeof(num_sent));
		for(cycle_num=0;cycle_num<4000000;cycle_num++){
			run(offerload);
		}
		ave_delay=total_delay/total_sent_num;
//		printf("ƽ��ʱ��%f\n",ave_delay);
//		fprintf(fp1,"%f\n",ave_delay);
		ave_delayingate=total_delayingate/total_sent_num;
//		printf("ƽ������ʱ��%f\n",ave_delayingate);
//		fprintf(fp6,"%f\n",ave_delayingate);
        ave_delayingate=total_delayingate/total_sent_num;
//		printf("ƽ�����ڵȴ�ʱ��%f\n",ave_delayingate);
//		fprintf(fp6,"%f\n",ave_delayingate);
		ave_busyperiod=total_busyperiod/cycle_num;
//		printf("ƽ��æ�ڳ���%f\n",ave_busyperiod);
//		fprintf(fp2,"%f\n",ave_busyperiod);
		ave_vacanum=(1.0*total_vacanum)/total_sent_num;
//      printf("ƽ��������������%f\n",ave_vacanum);
//		fprintf(fp3,"%f\n",ave_vacanum);
		ave_numingate=1.0*total_numingate/total_arr_num;
//		printf("ƽ�����ڵȴ�����%f\n",ave_numingate);
//		fprintf(fp8,"%f\n",ave_numingate);
        ave_numoutgate=1.0*total_numoutgate/total_arr_num;
//        printf("ƽ���������%f\n",ave_numoutgate);
//        fprintf(fp8,"%f\n",ave_numoutgate);
        ave_sentnumincycle=1.0*total_sent_num/nonzero_cycle;
//        printf("ƽ��ÿ�����ڷ�����������㣩%f\n",ave_sentnumincycle);
//        fprintf(fp9,"%f\n",ave_sentnumincycle);
        for (j=0;j<25;j++){
//        printf("���ڸ����ֲ�%f\n",1.0*num_ingate[j]/total_arr_num);
//        fprintf(fp5,"%f\n",1.0*num_ingate[j]/total_arr_num);
        }
	    for(j=0;j<300;j++){
//		printf("��������ֲ�%f\n",1.0*num_outgate[j]/total_arr_num);
//		fprintf(fp6,"%f\n",1.0*num_outgate[j]/total_arr_num); 
        }
		for(j=0;j<20;j++){
//		printf("vacation�����ֲ�%f\n",1.0*vacation_num[j]/total_sent_num);
//		fprintf(fp7,"%f\n",1.0*vacation_num[j]/total_sent_num);	
		} 
		for(j=0;j<=M;j++){
//			printf("����ʱ�����ڰ������ֲ�%f\n",1.0*num_sent[j]/cycle_num);
			
		}
		fprintf(fp2,"%f\n",1.0*num_sent[0]/cycle_num);
        fprintf(fp3,"%f\n",1.0*num_sent[1]/cycle_num);
//        fprintf(fp4,"%f\n",1.0*num_sent[2]/cycle_num);
//        fprintf(fp5,"%f\n",1.0*num_sent[3]/cycle_num);
//        fprintf(fp7,"%f\n",1.0*num_sent[4]/cycle_num);
    }
//	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
//	fclose(fp4);
//	fclose(fp5);
//	fclose(fp6);
//	fclose(fp7);
//	fclose(fp8);
//	fclose(fp9);
	
} 

