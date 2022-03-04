//Author:Berk Limoncu ID:2243541
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 20
#define D 10
#define Q 11

struct sensorData{
	int id;								//ID of the Sensor.
	int startTime;						//Start Time Of the sensor.
	int endTime;						//End Time of the sensor.
	int totalTime;						//End Time-Start Time
};

struct sensorDataRecord{
	struct sensorData *sensor;			//Arrays of sensors.
	int size;							//Size of the sensor whose working hour is >120 secs.
	char *fullPatern; 					//Full sequence of sensor that I'm going to store.
};

typedef struct sensorDataRecord Sensor;


Sensor createSensorData(char *);
int searchPattern(char *,char *,int ,int );
void printPattern(Sensor,int,int);

int main()
{
	char fileName[MAX],pattern[MAX];
	
	printf("Enter a file name > ");			//Getting file name from user.
	gets(fileName);
	
	struct sensorDataRecord s;
	s=createSensorData(fileName);			//Sending file name and it returns snesorDataRecord type of data.

	

	printf("Create a pattern now.\n0. Watering the flowers\n1. Sleeping\n2. Eating\n3. Relaxing\n4. Studyin\n5. Washing\n6. Cooking\n7. Bathroom\n8. Getting Dressed\n9. Entering/Leaving\nEnter a pattern:");
	gets(pattern);
	
	int startIndex=searchPattern(pattern,s.fullPatern,D,Q);		//StartIndex of pattern in the full pattern if pattern exist it returns index different than -1 . If it doesnt exist it returns -1.
	int endIndex=startIndex+strlen(pattern);				   //EndIndex of the pattern actually means = startIndex + lengthOfString(pattern)
	
	if(startIndex!=-1)										//If the pattern exist in the full pattern.
		printPattern(s,startIndex,endIndex);
	else
		printf("Pattern Doesn't Exist!");					//If the pattern doesnt exist in the full pattern
	
	return 0;
}

Sensor createSensorData(char *fileName)
{
	FILE *myFile;
	myFile=fopen(fileName,"r");		
	
	if(myFile==NULL)	//In case of file can't be read.
	{
		printf("Failed!\n");
		exit(0);
	}
	
	struct sensorData temp; //Temporary data for reading from .txt file.
	char status[3];			//Temporary data for reading from .txt file.
	
	int sizeOfData=0;		//Size of the data whose worked hour is >120 secs. I'm using this later to allocate memory.
	
	
	while(!feof(myFile))
	{
		fscanf(myFile,"%d       %s       %d",&temp.id,status,&temp.startTime);
		fscanf(myFile,"%d       %s       %d",&temp.id,status,&temp.endTime);
		
		if(temp.endTime>temp.startTime && temp.endTime-temp.startTime>120)					//EndTime > StartTime means started today and ended today.  AND . EndTime-StartTime>120 . If this is true then increment the sizeofdata that I'm going to allocate.
			sizeOfData++;
		else if(temp.startTime>temp.endTime &&temp.startTime-86399+temp.endTime >120)		//StartTime > EndTime means started yesterday and ended today. AND. StartTime-86399(23*3600+59*60+59) +EndTime > 120. If this is true then increment the sizeofdata that I'm going to allocate.
			sizeOfData++;
	}
	fclose(myFile);
	
	
	myFile=fopen(fileName,"r");
	
	if(myFile==NULL)			//In case of file can't be read.
	{
		printf("Failed!\n");
		exit(0);
	}
	

	Sensor s;																		//It's going to be returned.
	s.sensor=(struct sensorData*)malloc(sizeOfData*sizeof(struct sensorData));	    //Array of sensor whose working hour is >120 secs.
	s.size=sizeOfData; 															   //Size of the sensor data whose working hour is>120 secs.
	s.fullPatern=(char *)malloc(sizeOfData*sizeof(char));						  //Full pattern.
		
	int i=0;
	while(!feof(myFile))															
	{
		fscanf(myFile,"%d       %s       %d",&temp.id,status,&temp.startTime);			//I'm reading 2 line because I know multiple sensor can't work on the same time. In these line, I'm actually reading startTime of sensor.
		fscanf(myFile,"%d       %s       %d",&temp.id,status,&temp.endTime);			//I'm reading 2 line because I know multiple sensor can't work on the same time. In these line, I'm actually reading endTime of sensor.
		
		if(temp.endTime>temp.startTime && temp.endTime-temp.startTime>120)			//EndTime > StartTime means started today and ended today.  AND . EndTime-StartTime>120 .	If this is true than place datas into structs of tarray that I allocated memory of it.
		{
			s.fullPatern[i]=temp.id+'0'; //Int to Char conversion							//While I'm creating sensorData I'm also storing fullPattern of the sequence of sensor data line by line.
			s.sensor[i].id=temp.id;
			s.sensor[i].startTime=temp.startTime;
			s.sensor[i].endTime=temp.endTime;
			s.sensor[i].totalTime=temp.endTime-temp.startTime;
			i++;
		}
		else if(temp.startTime>temp.endTime &&temp.startTime-86399+temp.endTime >120)	//StartTime > EndTime means started yesterday and ended today. AND. StartTime-86399(23*3600+59*60+59) +EndTime > 120. If this is true than place datas into structs of tarray that I allocated memory of it.
	
		{
			
			s.fullPatern[i]=temp.id+'0'; //Int to Char conversion				//While I'm creating sensorData I'm also storing fullPattern of the sequence of sensor data line by line.
			s.sensor[i].id=temp.id;
			s.sensor[i].startTime=temp.startTime;
			s.sensor[i].endTime=temp.endTime;
			s.sensor[i].totalTime=temp.startTime-86399+temp.endTime;
			i++;
		}
	}	
	
	s.fullPatern[sizeOfData]='\0'; //To represent end of string.
	
	return s;
}

int searchPattern(char *P,char *T,int d,int q) 
{ 
   int n = strlen (T);			//Length of full pattern
   int m = strlen (P);			//Length of pattern that we are searching
   int p = 0;
   int t = 0;
   int h = ((int) pow(d,m-1)) %q;	
   int i, j;
   
   for (i=0;i<m;i++)
   {
      p= (d * p + P[i]) % q;
      t= (d * t + T[i]) % q;
   }
   
   for (i=0;i<=n-m;i++)
   {
      if (p==t)
	  {
         for (j=0;j<m;j++)
		 {
            if (T[i+j]!=P[j])
            	break;
         }
         
		 if (j==m)
            return i;
      }
      
	  if (i<n-m)
	  {
         t= (d * (t - T[i] * h) + T[i + m]) % q;
         
		 if (t<0)
            t= (t + q);
      }
   }
   return -1; //In case of the pattern doesnt exist in the full pattern.
}

void printPattern(Sensor s,int startIndex,int endIndex)
{
	char sensorNames[D][MAX] = {									//To access easily activities.
								"Watering the flowers",
								"Sleeping",
								"Eating",
								"Relaxing",
								"Studying",
								"Washing",
								"Cooking",
								"Bathroom",
								"Getting Dressed",
								"Entering/Leaving"
							};
	
	int i,totalSecondOfPattern=0; //i=iterator				//TotalSecondPattern total time of the pattern in seconds.
	
	printf("Pattern is created: ");
	
	for(i=startIndex;i<endIndex;i++)		//for example if input is "172" and full pattern is 3172899713627541 startIndex will be 1 and endIndex will be 1+lengthof(pattern)=4
	{
		if(i>startIndex)					//To avoid first '>'
			printf(" > ");
		printf("%s ",sensorNames[i]);
		totalSecondOfPattern+=s.sensor[i].totalTime;	//Adding all duration of the datas 
	}
	
	int hour,minute,second;
	
	hour=totalSecondOfPattern/3600;								//Typical sec->hour conversion.
	minute=(totalSecondOfPattern-hour*3600)/60;					//Typical sec->minute conversion.
	second=(totalSecondOfPattern-hour*3600-minute*60);			//Typical sec->sec conversion.	
		
	printf(":%d hour(s), %d minute(s), %d second(s)",hour,minute,second);
}
//Author:Berk Limoncu ID:2243541
