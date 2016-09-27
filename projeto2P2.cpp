#include <locale.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
using namespace std;
#include "despesas.h"
enum Escolhas {CRIAR=1, TELA, ARQUIVOTEXTO, ATUALIZAR, NOVO, APAGAR, FIM};
//------------------------------------------------------------------------
Escolhas enterChoice()
{
    int menuChoice;
    cout << "\nMenu:" << endl
         << "1 - Cria registros vazios no arquivo\n"
         << "2 - Lista despesas na tela\n"
         << "3 - Armazena os dados no arquivo texto \"financeiro.txt\"\n"
         << "4 - Atualiza despesas que ja contenham informações\n"
         << "5 - cadastra uma nova despesa\n"
         << "6 - Informar que conta já foi paga\n"
         << "7 - Fim do programa\n"
         << "Opção: ";
    cin >> menuChoice;
    return (Escolhas) menuChoice;
}
//------------------------------------------------------------------------
void create(fstream &f)
{
    despesa despesapaga = {0, "", 0.0};

    f.seekp(0);
    for (int i = 0; i < 100; i++)
        f.write((const char *)(&despesapaga),sizeof(despesa));
}
//------------------------------------------------------------------------
void relatorio(ostream &output, const despesa &c)
{
    output << setiosflags(ios::left)
           << setw(10) << c.numero
           << setw(30) << c.nome
           << setw(10) << setprecision(2) << resetiosflags(ios::left)
           << setiosflags(ios::fixed | ios::showpoint) << c.valor << '\n';
        
}
//------------------------------------------------------------------------
void Listardespesa(fstream &f)
{
    despesa c;
    float cont = 0;
    cout << setiosflags(ios::left)
         << setw(10) << "Número"
         << setw(30) << "Descrição"
         << resetiosflags(ios::left) << setw(10) << "Valor" << endl;

    f.seekp(0);
    f.read((char *)(&c),sizeof(despesa));
    while(! f.eof())
    {
		if(c.numero != 0){
            relatorio(cout,c);
            cont = cont+ c.valor;
            
    	}
        f.read((char *)(&c),sizeof(despesa));
    }
    
    cout<<cont;
}
//------------------------------------------------------------------------
void textFile(fstream &f)
{
    despesa c;
    ofstream outPrintFile("despesas.txt",ios::out);

    if(! outPrintFile)
    {
        cerr << "Arquivo financeiro.txt não pode ser aberto." << endl;
        exit(1);
    }

    outPrintFile << setiosflags(ios::left)
                 << setw(10) << "Número"
                 << setw(30) << "Descrição"
                 << resetiosflags(ios::left) << setw(10) << "Valor" << endl;

    f.seekp(0);
    f.read((char *)(&c),sizeof(despesa));
    while(! f.eof())
    {
        if(c.numero != 0)
            relatorio(outPrintFile,c);
        f.read((char *)(&c),sizeof(despesa));
    }
    outPrintFile.close();
}
//------------------------------------------------------------------------
int Testacadastro(string msg)
{
    int desp;

    do
    {
        cout << msg << " (1 - 100): ";
        cin >> desp;
    }
    while (desp < 1 || desp > 100);

    return desp;
}
//------------------------------------------------------------------------
void Alterar(fstream &f)
{
    int desp;
    despesa c;
    float alteracao;

    desp = Testacadastro("Despesa a ser atualizada");
    f.seekp((desp-1)*sizeof(despesa)); // posicionar na conta desejada
    f.read((char *)(&c),sizeof(despesa)); // ler dados da conta
    if(c.numero != 0) // conta contem informacao?
    {
        relatorio(cout,c);
        cout << "\nEntre com adição de valor (+) ou retirada de valor (-): ";
        cin >> alteracao;
        c.valor += alteracao;
        relatorio(cout,c);
        f.seekp((desp - 1) * sizeof(despesa)); // posicionar na conta desejada
        f.write((const char *)(&c),sizeof(despesa)); // atualizar
    }
    else
        cerr << "Despesa #" << desp << " não possui informação." << endl;
}
//------------------------------------------------------------------------
void newRecord(fstream &f)
{
    int numdesp;
    despesa c;

    numdesp = Testacadastro("ORDEM DA DESPESA: ");
    f.seekp((numdesp-1) * sizeof(despesa)); // posicionar na conta desejada
    f.read((char *)(&c),sizeof(despesa)); // ler dados da conta
    if(c.numero == 0)
    {
        cout << "Nome da despesa: ";
        cin >> c.nome;
        cout<<"Valor da despesa: ";
        cin>> c.valor;
        c.numero = numdesp;
        f.seekp((numdesp - 1) * sizeof(despesa)); // posicionar na conta desejada
        f.write((const char *)(&c),sizeof(despesa)); // atualizar
    }
    else
        cerr << "Ordem de despesa #" << numdesp << " ja possui informação." << endl;
}
//------------------------------------------------------------------------
void deleteRecord(fstream &f)
{
    int desp;
    despesa c, despesapaga = {0, "", 0.0};

    desp = Testacadastro("Despesa a ser paga");
    f.seekp((desp-1) * sizeof(despesa));
    f.read((char *)(&c),sizeof(despesa));
    if(c.numero != 0)
    {
        f.seekp((desp - 1) * sizeof(despesa));
        f.write((const char *)(&despesapaga), sizeof(despesa));
        cout << "Conta #" << desp << " apagada." << endl;
    }
    else
        cerr << "Despesa #" << desp << " já foi paga." << endl;
}
//------------------------------------------------------------------------


int main()
{
    setlocale(LC_ALL,"portuguese");

    Escolhas opcao;
    fstream inOutDespesas("financeiro.txt", ios::in | ios::out);

    if(! inOutDespesas)
    {
        cerr << "financeiro.txt não pode ser aberto." << endl;
        exit (1);
    }

    while ((opcao = enterChoice()) != FIM)
    {
        switch (opcao)
        {
        case CRIAR:
            create(inOutDespesas);
            break;
        case TELA:
            Listardespesa(inOutDespesas);
            break;
        case ARQUIVOTEXTO:
            textFile(inOutDespesas);
            break;
        case ATUALIZAR:
            Alterar(inOutDespesas);
            break;
        case NOVO:
            newRecord(inOutDespesas);
            break;
        case APAGAR:
            deleteRecord(inOutDespesas);
            break;
        default:
            cerr << "Opção incorreta\n";
            break;
        }
        inOutDespesas.clear();
    }
    return 0;
}
