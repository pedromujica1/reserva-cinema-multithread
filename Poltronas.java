//Classe Poltronas
/* IFPR - Campus Cascavel
Disciplina: Programação Orientada à Objetos
Professor: Nelson Bellincanta */

import java.util.Arrays;//Importa a biblioteca Arrays

import java.util.Scanner; //Importa a classe Scanner.


public class Poltronas {//Início da classe Poltronas
    public static void main(String[]args){//Declaração e inicialização do metódo main

        boolean[] poltronas1 = {false,false,false,false,false};//declaração e inicialização da array com os assentos da classe primária

        boolean[] poltronasEconomicas = {false,false,false,false,false};//declaração e inicialização da array com os assentos da classe economica

        boolean[] poltronasResultantes = new boolean[10];//Declaração e inicialização da array que vai receber a soma de poltronas1 e poltronasEconomicas  

        Scanner dados= new Scanner(System.in); //Inicia a classe Scanner para entrada de dados.
        Scanner respostaLotado= new Scanner(System.in); //Inicia a classe Scanner para entrada de outros dados.

        boolean verificador = true;//Variável para manter o while executando

        int index1=0;//Variável para contar quantas vezes assentos primarios foram compradas
        int index2=0;//Variável para contar quantas vezes assentos primarios foram comprados

        while (verificador){
            int contAssentosEconomica = 5;
            
            System.out.println("Insira um número: "+ "\n"); //Imprime a mensagem de escolha.
            System.out.println("Digite 1 para atribuir assentos na primeira classe "+ "\n");
            System.out.println("Digite 2 para atribuir assentos na classe econômica "+ "\n");
            int input = dados.nextInt(); //Declara e inicia a variável input
            

            if (input ==1 && index1<5) {//Início do if se o input for 1 e a variável contadora for menor que 5

                poltronas1[index1]=true;//Define o elemento da array na posição do index1 como true
                index1++;//Incremento da variável index1 para que seja atribuído uma nova posição na lista a cada iteração.
                 
                //metódo arraycopy que copia a array poltronas1 para a array poltronasResultantes
                //ordem dos argumentos(Object arrayOriginal, int posicao_arrayOriginal, Object arrayDestino,int posicaoarrayDestino, int tamanho_ArrayOriginal)
                System.arraycopy(poltronas1, 0, poltronasResultantes, 0, poltronas1.length);
                //metódo arraycopy que copia a array poltronasEconomicas para a Array poltronasResultantes  
                System.arraycopy(poltronasEconomicas, 0, poltronasResultantes, poltronas1.length,poltronasEconomicas.length);  
                        
                System.out.println("Você escolheu a classe primária");//Imprime a mensagem de qual classe foi atribuída

                System.out.println("Poltrona atribuída: "+(index1)+"\n");//Imprime a mensagem do número de poltrona atribuída

                System.out.println("Gráfico de poltronas: "+Arrays.toString(poltronasResultantes)); //Imprime a Array poltronas resultantes
            }//Fim do if


            else if (input == 1 && index1 >=5){//Início do if caso o input seja 1 e TODAS as poltronas da classe primária estajam compradas (index1>=5)

                //Imprime a mensagem da lotação de assentos na primeira classe
                System.out.println("A Primeira classe está cheia");


                //Imprime a mensagem se o usuário deseja comprar uma poltrona na classe econômica.
                System.out.println("Deseja comprar uma poltrona na classe econômica? (s/n)");
                //Declara e inicialização da variável que lê o input do usuário
                String resposta1 = respostaLotado.nextLine();

                if (resposta1.equals("s") ){//Início do if caso o cliente queira comprar assentos na classe econômica
                    
                    
                    int IndexRealClasse2 = (index2+contAssentosEconomica);//Variável IndexRealClasse2 para descobrir a posição da classe econômica na Array poltronasResultantes
                    //se index2 é 0, contAssentosEconomicas é 5. O assento da classe econômica vai estar na posição 5 da array poltronasResultantes
                    
                    if (poltronasResultantes[IndexRealClasse2] == false){//Inicio do if caso o assento na classe econômica esteja desocupado
                        

                        poltronasResultantes[IndexRealClasse2]=true;//Define o elemento da array na posição do IndexRealClasse2 como true, ou seja define a poltrona como comprada
                        
                        
                        boolean[] novasPoltronas = Arrays.copyOfRange(poltronasResultantes, 5, 10);//copia o array poltronasResultantes da posição 5 a 10

                        poltronasEconomicas=novasPoltronas;//Array poltronasEconomicas recebe array novasPoltronas para não haver divergência nos gráficos

                        System.out.println("Você comprou a poltrona "+(IndexRealClasse2+1)+ " na classe econômica"+"\n");//Imprime a mensagem de qual poltrona foi comprada
                        System.out.println("Gráfico de poltronas: "+Arrays.toString(poltronasResultantes)+"\n");//Mostra o gráfico de poltronas
                        
                        index2++;//Incremento da variável contador index2++ para atualizar posição que dever ser atribuida
                        contAssentosEconomica--;//Decrimento do contAssentosEconomica em -1 para ser somado com index2 e imprimir o número correto da  poltrona comprada
                    }//Fim do if
                }//Fim do if
                

                
                if (resposta1.equals("n")){//Inío do if caso o cliente não queira comprar assentos na classe econômica
                    //Impressão da mensagem
                    System.out.println("O próximo voo parte em 3 horas.");
                    break;//comando break para encerrar o programa
                }//Fim do if
            }

            if (input ==2 && index2<=4) {//If caso o usuário queira comprar assentos na classe economica e ainda tenha assentos disponíveis nesse classe (index2<=4)
                
        
                poltronasEconomicas[index2]=true;//Define o elemento da array na posição do index2 como true
                index2++;//Incremento da variável index2 para que seja atribuído uma nova posição na array a cada iteração.
                contAssentosEconomica--;//Decrimento do contAssentosEconomica em -1 para ser somado com index2 e imprimir o número correto da  poltrona comprada
                
                //metódo arraycopy para copiar a array poltronasEconomicas para a array poltronasResultantes
                System.arraycopy(poltronasEconomicas, 0, poltronasResultantes, poltronas1.length,poltronasEconomicas.length); 
                //metódo arraycopy para copiar a array poltronas1 para a array poltronasResultantes 
                System.arraycopy(poltronas1, 0, poltronasResultantes, 0, poltronas1.length); 

                //Imprime mensagem da classe atibúida
                System.out.println("Você escolheu a classe econômica");

            
                //Imprime a mensagem da poltrona escolhida com a soma da variável contadora index2 e contAssentosEconomica que começa em 5 e decresce a cada iteração
                System.out.println("Poltrona escolhida: "+ (index2+contAssentosEconomica)+"\n");

                //Imprime o gráfico das poltronas disponíveis e ocupadas
                System.out.println("Gráfico de poltronas: "+Arrays.toString(poltronasResultantes)+"\n");
            }//Fim do if

            else if(input ==2 && index2>4){//Início do if caso o input seja 2 e TODAS as poltronas da classe econômica estajam compradas (index2>=5)
                //Imprime a mensagem da lotação da classe economica
                System.out.println("A classe econômica está cheia");
            
                //imprime a mensagem de escolha do usuário
                System.out.println("Deseja comprar uma poltrona na classe 1? (s/n)");
                //variável resposta que lê o input do usuario
                String resposta = respostaLotado.nextLine();
                

                if (resposta.equals("s")){//Se a resposta for a string s que indica sim, o seguinte if é executado

                   
                    if (poltronas1[index1] == false){//se a poltrona na classe primária na posição do index1 estiver desocupada 
                        poltronas1[index1]=true;//Define a poltrona como ocupada, ou seja, define o elemento da array poltronas1 na posição do index1 como true
                        poltronasResultantes[index1]=true;//Define o elemento da array poltronasResultantes na posição do index1 como true para que não haja divergência nos gráficos
                        index1++;//incremento do index1 para que o sistema atribuia um novo assento ao usuário

                        //Imprime o número da poltrona comprada
                        System.out.println("Você comprou a poltrona "+(index1)+ " na classe primária"+"\n");
                        //imprime o gráfico de poltronas atual.
                        System.out.println("Gráfico de poltronas: "+Arrays.toString(poltronasResultantes)+"\n");
                        
                    }
                        
                }//Fim do if

                if (resposta.equals("n")){//se a resposta for a string n que indica não, o seguinte bloco de código é executado
                    System.out.println("O próximo voo parte em 3 horas");//Impressão da segunite mensagem
                    break;//FInaliza o programa
                }//fim do if
            }
        }
        dados.close();//Fechando objetos scanner
        respostaLotado.close();//Fechando objeto scanner 
    }//Fim do metódo main
}//Fim da classe Poltronas