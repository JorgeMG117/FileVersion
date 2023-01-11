#include "iostream"
#include <fstream>
#include "string"

#include <cstdlib>

#include <cstdio>


using namespace std;

enum Transf {
    nada,
    borrar,
    insert,
    sustit
};

//const LINE_LENGHT = 150;
const char DELIMITER = '-'; 


void compSec(string A, string B, int C[][150], Transf T[][150]){
    int n = A.length(); 
    int m = B.length();
    int x, y, z;
    for(int i = 0; i <= n; i++) C[i][0] = i; 
    for(int j = 0; j <= m; j++) C[0][j] = j;  

    for(int i = 1; i <= n; i++) {
        for(int j = 1; j <= m; j++){
            x = C[i-1][j] + 1; 
            y = C[i][j-1] + 1;
            
            if(A[i-1] == B[j-1]){
                z = C[i-1][j-1];        //no ha cambiado la cadena
            } else {
                z = C[i-1][j-1] + 1;    //ha cambiado la cadena
            }

            // cout << "x: " << x << endl;
            // cout << "y: " << y << endl;
            // cout << "z: " << z << endl;

            C[i][j] = min(min(x,y), z);
            
            if(C[i][j] == x){
                T[i][j] = borrar;
            }
            else if(C[i][j] == y){
                T[i][j] = insert;
            }
            else if(C[i][j] == z && A[i-1] == B[j-1]){
                T[i][j] = nada;
            }
            else {
                T[i][j] = sustit;
            }

        }
    }
}



void res(int i, int j, string B, Transf T[][150], ofstream& f){
    //cout << i << j;
    if(i == 0){
        for(int k = 0; k < j; k++) {
            cout << "Añadir " << B[k] << " en el lugar " << k << endl; 
            f << "i" << B[k] << k;
            f << DELIMITER;
        }
    } else if (j == 0){
        for(int k = i-1; k >= 0; k--) {
            f << "b" << k;
            f << DELIMITER;
            cout << "Borrar car.nº " << k << endl;
        }
    }
    else{
        if(T[i][j] == borrar){
            cout << "Borrar car.nº " << i << endl;
            f << "b" << i-1;
            f << DELIMITER;
            res(i-1, j, B, T, f);
        }
        else if(T[i][j] == insert){
            
            cout << "Insertar car.nº " << j << " de B tras la posicion " << i << endl;
            f << "i" << B[j-1] << i;
            f << DELIMITER;
            res(i, j-1, B, T, f);


            
        }
        else if(T[i][j] == nada){
            res(i-1, j-1, B, T, f);
            //cout << "Nada" << endl;
        }
        else {//T[i][j] = sustit;
            f << "s" << B[j-1] << i-1;
            f << DELIMITER;
            cout << "Sustit. car.nº " << i << " de A por nº " << j << " de B " << endl;
            res(i-1, j-1, B, T, f);
            
        }
    }
}


void modificar(string file ,int version){
    string fileSal = file + "_sal";

    ifstream fOld;
    string lineaOld;
    fOld.open(file + "_ant");
    getline(fOld, lineaOld);//Saltamos la linea de version
    fOld.close();
    int lastVersion = stoi(lineaOld);
    
    char op, c;
    string posicion;
    ifstream fCh;
    ofstream fSal;
    ifstream f;
     
    ofstream fAux;
    f.open(file);
    fAux.open(file + "_aux");
    fAux << f.rdbuf();
    f.close();
    fAux.close();

    for(int i = lastVersion; i > version; i--){
        cout << "Obteniendo version " << i << endl;
        //Obtener de file los cambios
        string fileCh = file + "_v" + to_string(i);

        
        fCh.open(fileCh);
        fSal.open(fileSal);
        f.open(file + "_aux");

        if(!fCh.is_open()){
            cout << "El fichero " << fileCh << " no se pudo abrir. La version " << version << "no esta disponible";
            return;
        }
        else if(!f.is_open()){
            cout << "El fichero " << file << " no se pudo abrir.";
            return;
        } else if(!fSal.is_open()){
            cout << "El fichero " << fileSal << " no se pudo abrir.";
            return;
        }

        string linea;
        string aux;
        string ej;
        bool saltoLinea = false;
        //char linea[150];
        getline(f, linea);
        while(fCh.get(op)){
            
            cout << endl;
            //cout << "Linea: " << linea << endl;
            if(op == 'i'){
                //coger el caracter a añadir
                getline(fCh, aux, DELIMITER);
              
                ej = aux.substr(0,1);
                posicion = aux.substr(1);

                if(ej == "\n"){
                    cout << "En salto linea" << endl;
                    //fSal << "\n" << endl;
                    fSal << "\n";
                    getline(fCh, aux);
                    continue;
                }
                else {
                    linea.insert(stoi(posicion), ej);
                }

                //cout << "Insertar " << ej << " en posicion " << posicion << endl;
                //cout << "Resultado: " << linea << endl;

                
            } else if(op == 'b'){
                //coger la posicion
                getline(fCh, posicion, DELIMITER);
                //cout << "Borrar en posicion " << posicion << endl;
                // //borrar
                //cout << "Pos: " << stoi(posicion) << endl;
                //cout << "Linea a borrar " << linea << endl;
                
                if(linea == "") 
                {   cout << "SALTOLINEA"<<endl;
                    saltoLinea = true;}
                linea.erase(stoi(posicion),1);
                
                  
                
                //cout << "Resultado: " << linea << endl;


            } else if(op == 's'){
                //coger el caracter a añadir
                fCh.get(c);
                //coger la posicion 
                getline(fCh, posicion, DELIMITER);

                linea[stoi(posicion)] = c;
                

                //cout << "Sustituir " << c << " en posicion " << posicion << endl;
                //cout << "Resultado: " << linea << endl;

            } else if(op == '\n'){
                //guardar la linea
                //for(long unsigned int i = 0; i < linea.length(); i++){
                //linea.erase(linea.find('x'), 1);
                //}
                //cout << "LineaModi: " << linea << endl;
                if(!saltoLinea){
                    cout << "esta si :( la pongo" << endl;
                    fSal << linea << endl;
                }
                saltoLinea = false;
                //borrar la linea
                linea = "";
                //coger nueva linea del fichero
                getline(f, linea);


            } else if(op == DELIMITER){
                cout << "DELIMITER" << endl;
            }
            
            else {
                cout << "Algo no va muy bien" << endl;
            }

        }

        f.close();
        fSal.close();
        fCh.close();
        
        //fSal.open(fileSal);
        //f.open(file + "_aux");
        
        //f << fSal.rdbuf();
        std::ifstream  src(fileSal, std::ios::binary);
        std::ofstream  dst(file + "_aux",   std::ios::binary);

        dst << src.rdbuf();
        src.close();
        dst.close();
            
        
        //f.close();
        //fSal.close();

    }

}




//queremos los cambios del nuevo al viejo para volver para atras
void leerFichero(string fichNew, string fichOld, string fichCh){
    int C[150][150];
    Transf T[150][150];
    
    ifstream fNew;
    fNew.open (fichNew);
    string lineaNew;

    ifstream fOld;
    fOld.open (fichOld);
    string lineaOld;

    ofstream fCh;
    fCh.open (fichCh);

    if(fCh.is_open()){
        if(fNew.is_open()){
            if(fOld.is_open()){
                getline(fOld, lineaOld);//Saltamos la linea de version
                while(getline(fNew, lineaNew) && getline(fOld, lineaOld)){
                    //cout << "cuidado";
                    //getline(fOld, lineaOld);
                    cout << "Nueva: " << lineaNew << endl;
                    cout << "Vieja: " << lineaOld << endl;
                    while(lineaNew == "" || lineaOld == ""){
                        if(lineaOld == "" && lineaNew == ""){
                            getline(fOld, lineaOld);
                            getline(fNew, lineaNew);
                            cout << "Nuevac: " << lineaNew << endl;
                            cout << "Viejac: " << lineaOld << endl;
                            fCh << endl;
                        }
                        else if(lineaOld == ""){
                            getline(fOld, lineaOld);
                            cout << "Nuevaa: " << lineaNew << endl;
                            cout << "Viejaa: " << lineaOld << endl;
                            fCh << "i" << '\n' << "0";
                            fCh << DELIMITER;
                            fCh << endl;
                        }
                        else{
                            getline(fNew, lineaNew);
                            cout << "Nuevab: " << lineaNew << endl;
                            cout << "Viejab: " << lineaOld << endl;
                            fCh << "b0";
                            fCh << DELIMITER;
                            fCh << endl;
                        }
                    }
                
                    //coparador
                    compSec(lineaNew, lineaOld, C, T);
                    //guardar el fichero de cambios
                    //cout << lineaNew << endl;
                    //cout << lineaOld << endl;
                    
                    res(lineaNew.length(), lineaOld.length(), lineaOld, T, fCh);
                    fCh << endl;
                    
                    lineaNew = "";
                    lineaOld = "";
                    //cout << "Bien" << endl;
                }
                //cout << "fin";
                //new largo borrar
                if(fOld.eof() && !fNew.eof()){
                    cout << "new largo borrar" << endl;
                    do{
                        cout << "LineaN: " << lineaNew << endl;
                        cout << "LongN: " << lineaNew.length() << endl;
                        if(lineaNew == ""){
                            fCh << "b0";
                            fCh << DELIMITER;
                        }
                        else { 
                            res(lineaNew.length(), 0, lineaNew, T, fCh); 
                        }
                        fCh << endl;
                    }while(getline(fNew, lineaNew));

                   
                }
                //old largo añadir
                else if(fNew.eof() && !fOld.eof()){
                    cout << "old largo añadir" << endl;
                    while(getline(fOld, lineaOld)){
                        cout << "LineaO: " << lineaOld << endl;
                        cout << "LongO: " << lineaOld.length() << endl;
                        if(lineaOld == ""){
                            fCh << "i" << '\n' << "0";
                            fCh << DELIMITER;
                        } else{
                            res(0,lineaOld.length(), lineaOld, T, fCh);
                        }

                        fCh << endl;
                    }
                    
                }

                //cout << "antes de cerrar";
                fOld.close();   
            }
            else {
                //error
                cout << "El fichero " << fichOld << " no se pudo abrir.";
            }
            fNew.close();
        }
        else{
            //error
            cout << "El fichero " << fichNew << " no se pudo abrir.";
        }
        fCh.close();
    }
    else {
        cout << "El fichero " << fichCh << " no se pudo abrir.";
    }
}

int main(int argc, char *argv[]){

    int op = 0;
    op = stoi(argv[1]);

    string fichero;
    cout << "Introduzca el nombre del fichero: " << endl;
    fichero = argv[2];

    if(op == 1){
        cout << "Introduzca la version a guardar: " << endl;
        int version = 0;
        version = stoi(argv[3]);
        if(version > 0){
            leerFichero(fichero, fichero + "_ant", fichero + "_v" + to_string(version));
        }  
        ifstream file;
        file.open(fichero);
        ofstream lastFile;
        lastFile.open(fichero + "_ant");
        lastFile << version << "\n" <<file.rdbuf();

    } else if(op == 2){
        //cout << "Introduzca la version a recuperar: " << endl;
        int version = 0;
        version = stoi(argv[3]);

        modificar(fichero, version);
    } else { cout << "Operación incorrecta" << endl;}

    return 0;
        



}







