#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
using namespace std;
char IR[4], R[4];
int CMP, IC , SI;
int i, j, k;
char Mem[300][4];
char buff[40];
int location;

void LOAD();
void INIT();
void START_EXECUTION();
void EXECUTE();
void READ();
void WRITE();
void TERMINATE();
void MOS();

fstream newfile;
fstream newofile;

void LOAD()
{
    int i,j,no=0,a=0;

    if (newfile.is_open()){
      string tp;
      int i=0;
      while(getline(newfile, tp)){
          for(i=0;i<tp.length();i++){
              buff[i]=tp[i];
          }

          int page_table[30];
          if (buff[0] == '$' && buff[1] == 'A' && buff[2] == 'M' && buff[3] == 'J'){
            cout<<"AMJ COMMAND FOUND:-"<<endl;
            no = 0;
            INIT();
            cout<<"Printing the instruction register content: "<<endl;
            for (i = 0; i < 4; i++){
                cout<<IR[i];
            }
            cout<<endl;
            cout<<"Printing the content of general purpose register: "<<endl;
            for (i = 0; i < 4; i++){
                cout<<R[i];
            }
            cout<<endl;
            cout<<"Printing the Compare flag"<<endl;
            cout<<CMP;
            cout<<endl;
            cout<<"Printing the instruction counter"<<endl;
            cout<<IC<<endl;
        }
        else if (buff[0] == '$' && buff[1] == 'D' && buff[2] == 'T' && buff[3] == 'A'){
            for (i = 0; i < 40; i++)
            {
                buff[i] = '\0';
            }
            cout<<"Starting Execution:- "<<endl;
            START_EXECUTION();
        }
        else if (buff[0] == '$' && buff[1] == 'E' && buff[2] == 'N' && buff[3] == 'D'){
            cout<<"$END COMMAND FOUND."<<endl;
            cout<<"Process Ended.\n"<<endl;
            continue;
        }
        else{
            int page_number = rand()%30;
            for (i = no; i < no + 10; i++){
                cout<<"Mem["<<i<<"] - ";
                for (j = 0; j < 4; j++){
                    Mem[i][j] = buff[a++];
                    cout<<Mem[i][j];
                }
                cout<<endl;
            }
            cout<<endl;
            no += 10;
            a = 0;
        }
        for(i=0;i<40;i++){
              buff[i]='\0';

          }
      }

      newfile.close(); //close the file object.
      newofile.close();
   }


}

void INIT(){
    for (i = 0; i < 100; i++){
        for (j = 0; j < 4; j++)
            Mem[i][j] = '\0';
    }

    for (i = 0; i < 4; i++){
        IR[i] = R[i] = '\0';
    }
    CMP = IC = 0;
}

void START_EXECUTION(){
    IC = 0;
    EXECUTE();
}

void EXECUTE(){
    cout<<endl;
    for (i = 0; i < 4; i++){
        IR[i] = Mem[IC][i];
    }
    for (i = 0; i < 4; i++){
        cout<<IR[i];
    }

    location = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);
    IC++;
    if(IR[0] != 'H')
        cout<<"\nloc = "<<location<<endl;

    if (IR[0] == 'L' && IR[1] == 'R'){
        cout<<"\nLR command - "<<endl;
        cout<<"Register Values: ";
        for (i = 0; i < 4; i++)
        {
            R[i] = Mem[location][i];
            cout<<R[i];
        }
        cout<<endl;
        EXECUTE();
    }
    else if (IR[0] == 'S' && IR[1] == 'R'){
        cout<<"\nSR command - "<<endl;
        cout<<"\ndata at loc "<<location<< " is :- ";
        for (i = 0; i < 4; i++){
            Mem[location][i] = R[i];
            cout<<Mem[location][i];
        }
        EXECUTE();
    }

    else if (IR[0] == 'C' && IR[1] == 'R'){
        cout<<"\nCR command - "<<endl;
        for (i = 0; i < 4; i++){
            if (R[i] == Mem[location][i]){
                CMP = 1;
            }

            else{
                CMP = 0;
                break;
            }
        }
        if (CMP) {
            cout<<"\nStrings ARE SAME"<<endl;
        }
        else {
            cout<<"\nStrings ARE NOT SAME"<<endl;
        }

        EXECUTE();
    }
    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        cout<<"\nBT command - "<<endl;
        if (CMP)
        {
            IC = location;
            cout<<"Value of IC - "<<IC;
        }
        EXECUTE();
    }
    else if (IR[0] == 'G' && IR[1] == 'D')
    {
        SI = 1;
        cout<<"\nGD command - "<<endl;
        MOS();
    }
    else if (IR[0] == 'P' && IR[1] == 'D')
    {
        SI = 2;
        cout<<"\nPD command - "<<endl;
        for (i = location; i < location + 10; i++)
        {
            cout<<"Mem["<<i<<"] : ";
            for (j = 0; j < 4; j++)
            {
                cout<<Mem[i][j];
            }
            cout<<endl;
        }
        MOS();
    }
    else if (IR[0] == 'O' && IR[1] == 'R'){
        int res,i=0;
        string s1,s2;
        for(i=0;i<4 && R[i]!='\0';i++){
            s1.push_back(R[i]);
        }
        int num1=stoi(s1);
        for(i=0;i<4;i++){
            s2.push_back(Mem[location][i]);
        }
        int num2=stoi(s2);
        res=(num1 | num2);
        cout<<"The OR of "<<num1<<" and "<<num2<<" is "<<res<<endl;
        for(i=0;i<4;i++){
            Mem[location][i]='\0';
        }
        string result=to_string(res);

        for(i=0;i<result.length();i++){
            Mem[location][i]=result[i];
        }
        EXECUTE();
    }
    else if (IR[0] == 'A' && IR[1] == 'D'){
        int res,i=0;
        string s1,s2;
        for(i=0;i<4 && R[i]!='\0';i++){
            s1.push_back(R[i]);
        }
        cout<<s1<<endl;
        int num1=stoi(s1);
        for(i=0;i<4;i++){
            s2.push_back(Mem[location][i]);
        }
        cout<<s2<<endl;
        int num2=stoi(s2);
        res=(num1 & num2);
        cout<<"The AND of "<<num1<<" and "<<num2<<" is "<<res<<endl;
        for(i=0;i<4;i++){
            Mem[location][i]='\0';
        }
        string result=to_string(res);

        for(i=0;i<result.length();i++){
            Mem[location][i]=result[i];
        }
        EXECUTE();
    }
    else if (IR[0] == 'H')
    {
        SI = 3;
        cout<<"\nProgram Terminated"<<endl;
        MOS();
    }
}

void READ(){

        string z;
        getline(newfile, z);
        for(i=0;i<z.length();i++){
            buff[i]=z[i];
        }
        k=0;
        for (i = location; i < location + 10; i++)
        {
            cout<<"Mem["<<i<<"] : ";
            for (j = 0; j < 4 && buff[k] != '\0'; j++)
            {
                Mem[i][j] = buff[k];
                k++;
                cout<<Mem[i][j];
            }
            cout<<endl;
        }
        for (i = 0; i < 40; i++)
        {
            buff[i] = '\0';
        }

    EXECUTE();
}

void WRITE(){
    k = 0;
    cout<<endl;
    for (i = 0; i < 40; i++)
    {
        buff[i] = '\0';
    }
    for (i = location; i < location + 10; i++)
    {
        for (j = 0; j < 4; j++)
        {
            buff[k] = Mem[i][j];
            cout<<buff[k];
            k++;
        }
    }
    cout<<endl;
    string s;
    if (newofile.is_open()) {

        s="";
        int i=0;
        while(buff[i]!='\0'){
            s.push_back(buff[i]);
            i++;
        }

        newofile<<s<<endl;
    }
    for (i = 0; i < 40; i++){
        buff[i] = '\0';
    }

    EXECUTE();
}

void TERMINATE(){
    newofile<<"\n\n";
}

void MOS(){
    switch (SI){
    case 1:
        READ();
        break;
    case 2:
        WRITE();
        break;
    case 3:
        TERMINATE();
        break;
    default:
        SI = 0;
    }
}



int main(){
   newfile.open("input.txt",ios::in);
   newofile.open("output.txt",ios::out);

   LOAD();
}

