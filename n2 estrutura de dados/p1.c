#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
time_t tstamp;
char id_s[50];
char val[256];
} LSen;

int compL_decrescente( const void *a, const void *b) {
LSen *lA = (LSen *)a;
LSen *lB = (LSen *)b;
if (lA->tstamp < lB->tstamp) return 1;
if (lA->tstamp > lB->tstamp) return -1;
return 0;
}

int main( int argc, char *argv[]) {
if (argc != 2) {
printf("Uso: %s <arq_ent>\n", argv[0]);
return 1;
}

FILE *f_ent = fopen(argv[1], "r");
if (f_ent == NULL) {
perror("Erro ao abrir o arquivo de entrada");
return 1;
}

LSen *todas_l = NULL;
int n_leit = 0;
int cap_leit = 100;

todas_l = (LSen *)malloc(cap_leit * sizeof(LSen));
if (todas_l == NULL) {
perror("Erro de alocacao");
fclose(f_ent);
return 1;
}

char linh[512];
long num_linha_atual = 0;
while (fgets(linh, sizeof(linh), f_ent) != NULL) {
num_linha_atual++;
if (n_leit >= cap_leit) {
cap_leit *= 2;
LSen *tmp = (LSen *)realloc(todas_l, cap_leit * sizeof(LSen));
if (tmp == NULL) {
perror("Erro de realocacao");
free(todas_l);
fclose(f_ent);
return 1;
}
todas_l = tmp;
}

if (sscanf(linh, "%ld %s %s", &todas_l[n_leit].tstamp,
todas_l[n_leit].id_s,
todas_l[n_leit].val) == 3) {
n_leit++;
} else {
fprintf(stderr, "Aviso: Linha %ld invalida ignorada: %s", num_linha_atual, linh);
}
}
fclose(f_ent);

if (n_leit == 0) {
printf("Nenhuma leitura valida encontrada no arquivo de entrada. Nada sera processado.\n");
free(todas_l);
return 0;
}

char **sensores_u = NULL;
int n_sen_u = 0;
int cap_sen_u = 10;

sensores_u = (char **)malloc(cap_sen_u * sizeof(char *));
if (sensores_u == NULL) {
perror("Erro aloc sen u");
free(todas_l);
return 1;
}

for (int i = 0; i < n_leit; i++) {
int enc = 0;
for (int j = 0; j < n_sen_u; j++) {
if (strcmp(todas_l[i].id_s, sensores_u[j]) == 0) {
enc = 1;
break;
}
}
if (!enc) {
if (n_sen_u >= cap_sen_u) {
cap_sen_u *= 2;
char **tmp = (char **)realloc(sensores_u, cap_sen_u * sizeof(char *));
if (tmp == NULL) {
perror("Erro realoc sen u");
for(int k=0; k<n_sen_u; k++) free(sensores_u[k]);
free(sensores_u);
free(todas_l);
return 1;
}
sensores_u = tmp;
}
sensores_u[n_sen_u] = strdup(todas_l[i].id_s);
if (sensores_u[n_sen_u] == NULL) {
perror("Erro aloc id sen");
for(int k=0; k<n_sen_u; k++) free(sensores_u[k]);
free(sensores_u);
free(todas_l);
return 1;
}
n_sen_u++;
}
}

for (int i = 0; i < n_sen_u; i++) {
char *s_at = sensores_u[i];

LSen *ls_do_sen = NULL;
int cont_s_ls = 0;
int cap_s_ls = 100;

ls_do_sen = (LSen *)malloc(cap_s_ls * sizeof(LSen));
if (ls_do_sen == NULL) {
perror("Erro aloc ls do sen");
continue;
}

for (int j = 0; j < n_leit; j++) {
if (strcmp(todas_l[j].id_s, s_at) == 0) {
if (cont_s_ls >= cap_s_ls) {
cap_s_ls *= 2;
LSen *tmp = (LSen *)realloc(ls_do_sen, cap_s_ls * sizeof(LSen));
if (tmp == NULL) {
perror("Erro realoc ls do sen");
free(ls_do_sen);
ls_do_sen = NULL;
break;
}
ls_do_sen = tmp;
}
ls_do_sen[cont_s_ls] = todas_l[j];
cont_s_ls++;
}
}

if (ls_do_sen != NULL) {
qsort(ls_do_sen, cont_s_ls, sizeof(LSen), compL_decrescente);

char nome_arq_saida[256];
snprintf(nome_arq_saida, sizeof(nome_arq_saida), "%s.txt", s_at);

FILE *arq_s = fopen(nome_arq_saida, "w");
if (arq_s == NULL) {
perror("Erro criar arq saida p/ sensor");
} else {
for (int k = 0; k < cont_s_ls; k++) {
fprintf(arq_s, "%ld %s %s\n",
ls_do_sen[k].tstamp,
ls_do_sen[k].id_s,
ls_do_sen[k].val);
}
fclose(arq_s);
}
free(ls_do_sen);
}
}

for(int i=0; i<n_sen_u; i++) {
free(sensores_u[i]);
}
free(sensores_u);
free(todas_l);

return 0;
}