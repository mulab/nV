#include<string>
#include<algorithm>
using namespace std;

namespace genTex
{
    FILE *f;

    string processInnerText(string text)
    {
        string res="";
        for (int i=0;i<text.size();i++)
        if (text[i]=='\n') res+="";else
        if (text[i]=='{' || text[i]=='}') res=res+"\\"+text[i];
        else res+=text[i];
        return res;
    }

    void initTex(string fineNale,string title)
    {
        f=fopen(("doc\\"+fineNale+".tex").c_str(),"w");
        fprintf(f,"\
\\documentclass[11pt]{article}\n\
\\usepackage{CJK}\n\
\\usepackage{indentfirst}\n\
\\usepackage{latexsym,bm}\n\
\\usepackage{amsmath}\n\
\\usepackage{amssymb}\n\
\\usepackage{fancybox}\n\
\\usepackage{listings}\n\
\\usepackage{multicol}\n\
\\usepackage{color}\n\
\\usepackage{graphicx}\n\
\\usepackage{paralist}\n\
\\usepackage[linesnumbered,lined,boxed,commentsnumbered,ruled]{algorithm2e}\n\
\\usepackage{engord}\n\
\\usepackage{booktabs}\n\
                ");

        fprintf(f,"\n\
\\begin{document}\n\
\\begin{CJK*}{GBK}{song}\n\
\\title{%s}\n\
\\maketitle\n\
                ",title.c_str());
    }


    void addText(const string text)
    {
        string buf="";
        for (int i=0;i<text.size();i++)
        if (text[i]=='\n') buf+="\n\n";
        else buf+=text[i];

        fprintf(f,buf.c_str());
    }

    void end()
    {
        fprintf(f,"\n\
\\end{CJK*}\n\
\\end{document}\n");
        fclose(f);
    }

}
