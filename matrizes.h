#ifndef MATRIZES_H
#define MATRIZES_H

#pragma once // usar para segurança (evitar que a mesma inclusao aconteca mais de uma vez)
#include <iostream>
#include <vector>
#include <cstring>
#include <math.h>

using namespace std;

template <typename T>
void print_vetor (vector<T>& v) {
    for (int i = 0; i < v.size(); ++i) cout << v[i] << ' ';
    cout << endl;
}

template <typename T>
void print_vetor (T v[], int tamanho) {
    for (int i = 0; i < tamanho; ++i) cout << v[i] << ' ';
    cout << endl;
}

vector<vector<double>> entra_matriz() {
    int n, m;
    cout << "linhas: "; cin >> n;
    cout << "colunas: "; cin >> m;

    vector<vector<double>> mat(n, vector<double>(m+1, 0));

    cout << "Entrada dos coeficientes:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << "M[" << i << "][" << j << "] = ";
            cin >> mat[i][j];
        }
    }

    cout << "Entrada do vetor resultado\n";
    for (int i = 0; i < n; ++i) {
        cout << "b[" << i << "] = ";
        cin >> mat[i][m];
    }

    return mat;
}

template <typename T> 
void print_matriz(vector<vector<T>>& matriz) {
    
    for (int i = 0; i < matriz.size(); ++i) {
        cout << "[ ";
        for (int j = 0; j < matriz[i].size()-1; ++j) {
            cout << matriz[i][j] << ' ';
        }
        cout << "| " << matriz[i][matriz[i].size()-1] << " ]" << endl;
    }
    cout << endl;
}

// note que a primeira linha eh 0
template <typename T> 
void switch_lines(vector<vector<T>>& matriz, int l1, int l2) {
    if (l1 >= matriz.size() || l2 >= matriz.size() || matriz[l1].size() != matriz[l2].size() ) return;

    for (int i = 0; i < matriz[l1].size(); ++i) {
        T aux = matriz[l1][i];
        matriz[l1][i] = matriz[l2][i];
        matriz[l2][i] = aux;
    }
}

template <typename T> 
void switch_lines(vector<T>& vetor, int l1, int l2) {
    if (l1 >= vetor.size() || l2 >= vetor.size()) return;
    T aux = vetor[l1];
    vetor[l1] = vetor[l2];
    vetor[l2] = aux;
}

// Lx <- Lx - c_sub*Ly
template <typename T>
void sublinhas(vector<vector<T>>& matriz, int lx, int ly, double c_sub) {
    if (lx >= matriz.size() || ly >= matriz.size() || matriz[lx].size() != matriz[ly].size() ) return;
    for (int i = 0; i < matriz[lx].size(); ++i) matriz[lx][i] -= c_sub*matriz[ly][i];
} 

// Lx <- Lx/c_div
template <typename T>
void divlinhas(vector<vector<T>>& matriz, int lx, double c_div) {
    if (lx >= matriz.size()) return;
    for (int i = 0; i < matriz[lx].size(); ++i) matriz[lx][i] /= c_div;
}

template <typename T>
vector<vector<T>> elim_gauss(vector<vector<T>>& matriz, bool piv = 0) {
    vector<vector<T>> out = matriz;

    for (int i = 0; i < out.size()-1; ++i) { // nao percorre coluna dos resultados
        
        if (piv) {
            // ind_max serah a linha maxima
            int ind_max = i;
            for (int j = i+1; j < out.size(); ++j) {
                if (out[j][i] > out[ind_max][i]) {
                    ind_max = j;
                }
            }

            switch_lines(out, i, ind_max);
        }

        double pivo = out[i][i];

        for (int j = i + 1; j < out.size(); ++j) { // laco para as linhas abaixo do pivo
            double sub = out[j][i]/pivo;

            // subtracao de linha pela outra aqui
            for (int k = i; k < out[j].size(); ++k) { // tirando a linha
                out[j][k] -= sub*out[i][k];
            } 
        }
    } 

    return out;
}

template <typename T>
vector<vector<T>> gauss_jordan(vector<vector<T>>& matriz, bool piv = 0) {
    vector<vector<T>> maux = elim_gauss(matriz, piv);

    for (int i = maux.size()-1; i >= 0; --i) {
        for (int k = i - 1; k >= 0; --k) {
            // ver se troco o sublinhas por um processo sem ser por funcao (mais rapido)
            sublinhas(maux, k, i, (double)(maux[k][i]/maux[i][i]));
        }
    }

    return maux;

}

// INPUT: NxN MATRIX
template <typename T>
vector<vector<T>> inversa_gj(vector<vector<T>>& matriz, bool piv = 0) {
    vector<vector<T>> maux(matriz.size(), vector<T>(matriz[0].size()*2, 0));

    for (int i = 0; i < matriz.size(); ++i) {
        maux[i][i + matriz.size()] = 1;
        for (int j = 0; j < matriz[i].size(); ++j) {
            maux[i][j] = matriz[i][j];
        }
    }

    maux = gauss_jordan(maux, piv);

    for (int i = 0; i < maux.size(); ++i) {
        divlinhas(maux, i, maux[i][i]);
    }


    vector<vector<T>> out(matriz.size(), vector<T>(matriz[0].size(), 0));
    
    for (int i = 0; i < matriz.size(); ++i) {
        for (int j = matriz.size(); j < matriz[i].size()*2; ++j) {
            out[i][j-matriz.size()] = maux[i][j]; 
        }
    }

    return out;
}

template <typename T>
double mult_diag_prin(vector<vector<T>>& matriz) {
    double res = 1;
    for (int i = 0; i < matriz.size(); ++i) res *= matriz[i][i];
    return res;
}

template <typename T>
double det(vector<vector<T>>& matriz, bool piv = 0) {
    vector<vector<T>> maux = elim_gauss(matriz, piv);
    return mult_diag_prin(maux);
}

template <typename T>
double det_gj(vector<vector<T>>& matriz, bool piv = 0) {
    vector<vector<T>> maux = gauss_jordan(matriz, piv);
    return mult_diag_prin(maux);
}

template <typename T>
vector<vector<T>> fat_LU(vector<vector<T>>& matriz, bool piv = 0) {

    vector<T> resultado;
    vector<vector<T>> matriz_sem_resul = matriz; 
    vector<vector<T>> L;

    for (int i = 0; i < matriz.size(); ++i) {
        L.push_back(vector<T>(matriz[i].size()-1, 0));
    } L[L.size()-1][L[L.size()-1].size()-1] = 1;

    for (int i = 0; i < matriz.size(); ++i) {
        resultado.push_back(matriz[i][matriz[0].size()-1]);
        matriz_sem_resul[i][matriz[0].size()-1] = 0;
    }

    // OK: a separacao deu certo
    // print_matriz(matriz_sem_resul);
    // print_vetor(resultado);

    // fazer o gauss para a matriz sem resultado
    for (int i = 0; i < matriz_sem_resul.size()-1; ++i) { // nao percorre coluna dos resultados
        
        if (piv) {
            int ind_max = i;
            for (int j = i+1; j < matriz_sem_resul.size(); ++j) {
                if (matriz_sem_resul[j][i] > matriz_sem_resul[ind_max][i]) {
                    ind_max = j;
                }
            }

            // realizando troca
            switch_lines(matriz_sem_resul, i, ind_max);
            switch_lines(resultado, i, ind_max);
            
        }
        
        double pivo = matriz_sem_resul[i][i];
        L[i][i] = 1;
        for (int j = i + 1; j < matriz_sem_resul.size(); ++j) { // laco para as linhas abaixo do pivo
            double sub = matriz_sem_resul[j][i]/pivo;
            L[j][i] = sub;

            // subtracao de linha pela outra aqui
            for (int k = i; k < matriz_sem_resul[j].size(); ++k) { // tirando a linha
                matriz_sem_resul[j][k] -= sub*matriz_sem_resul[i][k];
            } 
        }
    } 
    
    // vendo como fica a matriz depois do processo

    // print_matriz(matriz_sem_resul);
    // print_matriz(L);
    
    // ou seja, nosso U = matriz_sem_resul, porem nao se deve considerar a ultima coluna
    // e nosso L ja vem sem a ultima coluna

    // agora resolver o sistema
    // primeiro o Ly = b

    vector<T> y;
    y.push_back(resultado[0]);
    int j = 1;

    for (int i = 1; i < L.size(); ++i) {
        double s = resultado[j];
        for (int k = 0; k < j; ++k) {
            s -= L[i][k]*y[k];
        }
        j++;
        y.push_back(s);
    }

    // vetor y resolvido
    // print_vetor(y);

    for (int i = 0; i < matriz.size(); ++i) {
        matriz_sem_resul[i][matriz[i].size()-1] = y[i];
    }

    return matriz_sem_resul;
}

#endif
