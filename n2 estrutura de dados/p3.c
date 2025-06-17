#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

time_t conv_tstamp(int d, int m, int a, int h, int min, int s) {
struct tm t = {0};
t.tm_year = a - 1900;
t.tm_mon = m - 1;
t.tm_mday = d;
t.tm_hour = h;
t.tm_min = min;
t.tm_sec = s;
t.tm_isdst = -1;

time_t ts = mktime(&t);
return ts;
}

time_t gera_tstamp_aleat(struct tm * inicio, struct tm * fim) {
time_t ts_ini, ts_fim;

ts_ini = mktime(inicio);
if (ts_ini == (time_t)-1) { return (time_t)-1; }

ts_fim = mktime(fim);
if (ts_fim == (time_t)-1) { return (time_t)-1; }

if (ts_ini > ts_fim) { return (time_t)-1; }

if (ts_fim - ts_ini + 1 <= 0) { return ts_ini; }

time_t ts_aleat = ts_ini + rand() % (ts_fim - ts_ini + 1);

return ts_aleat;
}

void gera_val_aleat(char *buf, const char *tipo_dado) {
if (strcmp(tipo_dado, "CONJ_Z") == 0) {
sprintf(buf, "%d", rand() % 10000);
} else if (strcmp(tipo_dado, "CONJ_Q") == 0) {
sprintf(buf, "%.2f", (float)rand() / (float)(RAND_MAX / 1000.0));
} else if (strcmp(tipo_dado, "TEXTO") == 0) {
int len = 5 + rand() % 12;
for (int i = 0; i < len; i++) {
buf[i] = 'a' + (rand() % 26);
}
buf[len] = '\0';
} else if (strcmp(tipo_dado, "BINARIO") == 0) {
sprintf(buf, "%s", (rand() % 2 == 0) ? "true" : "false");
} else {
sprintf(buf, "UNKNOWN_TYPE");
}
}

int main(int argc, char *argv[]) {
if ((argc - 1) % 2 != 0 || argc < 13) {
printf("Uso: %s <d_ini> <m_ini> <a_ini> <h_ini> <min_ini> <s_ini> ", argv[0]);
printf("<d_fim> <m_fim> <a_fim> <h_fim> <min_fim> <s_fim> ");
printf("<ID_S1>:<TIPO1> [<ID_S2>:<TIPO2> ...]\n");
printf("Tipos: CONJ_Z, CONJ_Q, TEXTO, BINARIO\n");
return 1;
}

srand((unsigned int)time(NULL));

struct tm ini_tm = {0};
ini_tm.tm_mday = atoi(argv[1]);
ini_tm.tm_mon = atoi(argv[2]) - 1;
ini_tm.tm_year = atoi(argv[3]) - 1900;
ini_tm.tm_hour = atoi(argv[4]);
ini_tm.tm_min = atoi(argv[5]);
ini_tm.tm_sec = atoi(argv[6]);
ini_tm.tm_isdst = -1;

struct tm fim_tm = {0};
fim_tm.tm_mday = atoi(argv[7]);
fim_tm.tm_mon = atoi(argv[8]) - 1;
fim_tm.tm_year = atoi(argv[9]) - 1900;
fim_tm.tm_hour = atoi(argv[10]);
fim_tm.tm_min = atoi(argv[11]);
fim_tm.tm_sec = atoi(argv[12]);
fim_tm.tm_isdst = -1;

if (mktime(&ini_tm) == (time_t)-1) {
fprintf(stderr, "Erro: Data/hora de inicio invalida.\n");
return 1;
}
if (mktime(&fim_tm) == (time_t)-1) {
fprintf(stderr, "Erro: Data/hora de fim invalida.\n");
return 1;
}
if (mktime(&ini_tm) > mktime(&fim_tm)) {
fprintf(stderr, "Erro: A data/hora de inicio nao pode ser posterior a data/hora de fim.\n");
return 1;
}

int n_args_sen = argc - 13;
if (n_args_sen <= 0) {
fprintf(stderr, "Erro: Nenhum sensor especificado.\n");
return 1;
}

FILE *arq_s = fopen("dados_brutos_gerados.txt", "w");
if (arq_s == NULL) {
perror("Erro ao criar o arquivo de saida");
return 1;
}

for (int i = 0; i < n_args_sen; i++) {
char *sen_tipo_s_original = strdup(argv[13 + i]);
char *sen_tipo_s = sen_tipo_s_original;
char *id_s = strtok(sen_tipo_s, ":");
char *tipo_d = strtok(NULL, ":");

if (id_s == NULL || tipo_d == NULL) {
fprintf(stderr, "Aviso: Formato invalido para sensor:tipo '%s'. Esperado ID_SENSOR:TIPO.\n", argv[13 + i]);
free(sen_tipo_s_original);
continue;
}

if (strcmp(tipo_d, "CONJ_Z") != 0 &&
strcmp(tipo_d, "CONJ_Q") != 0 &&
strcmp(tipo_d, "TEXTO") != 0 &&
strcmp(tipo_d, "BINARIO") != 0) {
fprintf(stderr, "Aviso: Tipo de dado '%s' para o sensor '%s' e invalido. Tipos validos: CONJ_Z, CONJ_Q, TEXTO, BINARIO. Ignorando este sensor.\n", tipo_d, id_s);
free(sen_tipo_s_original);
continue;
}

for (int j = 0; j < 2000; j++) {
time_t ts_aleat = gera_tstamp_aleat(&ini_tm, &fim_tm);
if (ts_aleat == (time_t)-1) {
break;
}

char val_aleat_s[256];
gera_val_aleat(val_aleat_s, tipo_d);

fprintf(arq_s, "%ld %s %s\n", (long)ts_aleat, id_s, val_aleat_s);
}
free(sen_tipo_s_original);
}

fclose(arq_s);
printf("Dados gerados em 'dados_brutos_gerados.txt'\n");

return 0;
}