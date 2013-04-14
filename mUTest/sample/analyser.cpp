#include<cstdio>
#include<fstream>
#include<algorithm>
#include<string>
#include<cstring>
#include<vector>

#include "genTex.h"

using namespace std;

char buf[10000];

string ch2str(char buf[])
{
    string res="";
    for (int i=0;buf[i];i++) res+=buf[i];
    return res;
}

string int2str(int d)
{
    string res="";
    if (d==0) return "0";
    for (;d;d/=10) res=(char)(d%10+'0')+res;
    return res;
}

void analyse(string funcName)
{
    string path="result\\"+funcName+".txt";
    FILE *f=fopen(path.c_str(),"r");

    vector<string> inputData,outputData,stdResult;
    vector<int> cmpResult;
    while (fgets(buf,10000,f)!=NULL)
    {
        inputData.push_back(ch2str(buf));
        fgets(buf,10000,f);
        outputData.push_back(ch2str(buf));
        fgets(buf,10000,f);
        stdResult.push_back(ch2str(buf));
        fgets(buf,10000,f);
        cmpResult.push_back(buf[0]-'0');
    }
    fclose(f);

    int passCnt=0;
    for (int i=0;i<cmpResult.size();i++) passCnt+=cmpResult[i];

    genTex::initTex(funcName,"maTHmU内核测试--"+funcName);

    genTex::addText("\n\\section{概况}\n");
    genTex::addText("测试数据数目: "+int2str(inputData.size())+"\n");
    genTex::addText("通过数据数目: "+int2str(passCnt)+"\n");
    genTex::addText("通过百分比: "+int2str((double)passCnt/inputData.size()*100)+"\n");
    
    genTex::addText("\n\\section{未通过数据}\n");
    for (int i=0;i<inputData.size();i++)
    if (!cmpResult[i])
    {
        genTex::addText((string)"\n\\subsection*{"+"Test"+int2str(i+1)+":}\n");
        genTex::addText("输入: ");
        genTex::addText("{"+genTex::processInnerText(inputData[i])+"}\n");
        genTex::addText("\\vspace{0.2cm}\n");
        genTex::addText("输出: ");
        genTex::addText("{"+genTex::processInnerText(outputData[i])+"}\n");
        genTex::addText("\\vspace{0.2cm}\n");
        genTex::addText("标准输出: ");
        genTex::addText("{"+genTex::processInnerText(stdResult[i])+"}\n");
        genTex::addText("\\vspace{0.2cm}\n");
    }
    genTex::end();
}

int main()
{
    ifstream flist("FileList.txt",ifstream::in);
    vector<string> fileName;
    string tmp;
    while (flist >> tmp) fileName.push_back(tmp);
    flist.close();
    for_each(fileName.begin(),fileName.end(),analyse);
}
