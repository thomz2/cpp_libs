#ifndef SORTS_H
#define SORTS_H

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

void counting_sort (vector<int>& v, const int limite) {
    int C[limite+1];
    vector<int>* output = new vector<int> (v.size(), 0);
    memset(C, 0, sizeof(C));
    for (int i = 0; i < v.size(); ++i) C[v[i]]++;
    for (int i = 1; i <= limite; ++i) C[i] += C[i-1];
    for (int i = 0; i < v.size(); ++i) (*output)[C[v[i]]-1] = v[i];
    v = *output;
}

void counting_sort (vector<int>& v) {
    int maximo = v[0];
    for (int i = 0; i < v.size(); ++i) if (v[i] > maximo) maximo = v[i];
    counting_sort (v, maximo);
}

// lista 2 questao 5 
void counting_sort_esp (vector<int>& v) { // sem o vetor auxiliar B
    
    int limite = v[0];
    for (int i = 0; i < v.size(); ++i) if (v[i] > limite) limite = v[i];
    
    int C[limite+1];
    memset(C, 0, sizeof(C));

    for (int i = 0; i < v.size(); ++i) C[v[i]]++;

    for (int i = 0, j = 0; i <= limite; ++i) {
        while (C[i]) {
            v[j++] = i;
            C[i]--; 
        }
    }   
}

// vector<int> quase_ordenacao (vector<int>& v) {

// }

// i eh o indice dos menores
// j eh o indice geral
template <typename T>
int particiona (vector<T>& v, int p, int r, int ind_piv = -1) {
    T pivo = ind_piv == -1 ? v[r] : v[ind_piv];
    int i = p;
    for (int j = p; j <= r; ++j) if (v[j] <= pivo) {
        T aux = v[j];
        v[j] = v[i];
        v[i] = aux;
        ++i;
    }
    return i - 1;
}

template <typename T>
int particiona (vector<T>& v){
    return particiona(v, 0, v.size()-1, v.size()-1);
} 

template <typename T>
void quick_sort(vector<T>& v, int a, int b) {
    if (a < b) {
        const int q = particiona(v, a, b);
        quick_sort(v, a, q-1);
        quick_sort(v, q+1, b);
    }
}

template <typename T>
void quick_sort(vector<T>& v) {
    quick_sort(v, 0, v.size()-1);
}

void radix_sort(vector<int>& v) {
    int val_max = v[0], exp = 1;

    for (int i = 1; i < v.size(); ++i) if (v[i] > val_max) val_max = v[i];
    
    while ((int)(val_max/exp) > 0) {
        int C[10] = {};
        
        for (int i = 0; i < v.size(); ++i) {
            int dig = (v[i] / exp) % 10;
            C[dig]++;
        }

        for (int i = 1; i < 10; ++i) C[i] += C[i-1];

        vector<int>* B = new vector<int> (v.size(), 0);

        for (int i = 0; i < v.size(); ++i) (*B)[--C[(int)((v[i] / exp) % 10)]] = v[i];
        // note que o que esta dentro da chave de C eh o digito atual de v[i]
        
        v = *B;
        exp *= 10;
    }
}

#endif
