#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

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

typedef struct {
time_t tstamp;
char id_s[50];
char val[256];
} LSenCon;

long get_line_offset(FILE *file, long line_num) {
fseek(file, 0, SEEK_SET);
char buffer[512];
long current_line = 0;
long offset = 0;
while (fgets(buffer, sizeof(buffer), file) != NULL) {
if (current_line == line_num) {
return offset;
}
offset = ftell(file);
current_line++;
}
return -1;
}

int main(int argc, char *argv[]) {
if (argc != 8) {
printf("Uso: %s <ID_SENSOR> <d> <m> <a> <h> <min> <s>\n", argv[0]);
return 1;
}

char *id_s_pro = argv[1];

for (int i = 0; id_s_pro[i] != '\0'; i++) {
if (!isalnum((unsigned char)id_s_pro[i]) && id_s_pro[i] != '_') {
fprintf(stderr, "Erro: ID do sensor '%s' contem caracteres invalidos. Use apenas letras, numeros e underscore.\n", id_s_pro);
return 1;
}
}

int d = atoi(argv[2]);
int m = atoi(argv[3]);
int a = atoi(argv[4]);
int h = atoi(argv[5]);
int min = atoi(argv[6]);
int s = atoi(argv[7]);

time_t tstamp_pro = conv_tstamp(d, m, a, h, min, s);
if (tstamp_pro == (time_t)-1) {
fprintf(stderr, "Erro: Data/hora invalida.\n");
return 1;
}

char nome_arq_sen[256];
snprintf(nome_arq_sen, sizeof(nome_arq_sen), "%s.txt", id_s_pro);

FILE *arq_s = fopen(nome_arq_sen, "r");
if (arq_s == NULL) {
perror("Erro ao abrir o arquivo do sensor");
return 1;
}

long num_l_arq = 0;
char linh_tmp[512];
while (fgets(linh_tmp, sizeof(linh_tmp), arq_s) != NULL) {
num_l_arq++;
}

if (num_l_arq == 0) {
fclose(arq_s);
printf("Arquivo do sensor vazio.\n");
return 0;
}

long low = 0;
long high = num_l_arq - 1;
LSenCon l_mais_perto;
double menor_dif = -1.0;
bool encontrado_exato = false;

while (low <= high) {
long mid = low + (high - low) / 2;

long pos_l_mid = get_line_offset(arq_s, mid);
if (pos_l_mid == -1) {
break;
}
fseek(arq_s, pos_l_mid, SEEK_SET);

LSenCon l_atual;
if (fgets(linh_tmp, sizeof(linh_tmp), arq_s) == NULL) {
break;
}

if (sscanf(linh_tmp, "%ld %s %s", &l_atual.tstamp,
l_atual.id_s,
l_atual.val) != 3) {
break;
}

double dif_atual = fabs((double)l_atual.tstamp - tstamp_pro);

if (menor_dif == -1.0 || dif_atual < menor_dif) {
menor_dif = dif_atual;
l_mais_perto = l_atual;
}

if (l_atual.tstamp == tstamp_pro) {
l_mais_perto = l_atual;
encontrado_exato = true;
break;
} else if (l_atual.tstamp > tstamp_pro) {
low = mid + 1;
} else {
high = mid - 1;
}
}

fclose(arq_s);

if (menor_dif != -1.0) {
printf("Sensor ID: %s\n", l_mais_perto.id_s);
printf("Timestamp: %ld\n", (long)l_mais_perto.tstamp);
printf("Valor: %s\n", l_mais_perto.val);

time_t ts_out = l_mais_perto.tstamp;
struct tm *inf = localtime(&ts_out);
char buf[80];
strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", inf);
printf("Data/Hora: %s\n", buf);
} else {
printf("Nenhuma leitura encontrada.\n");
}

return 0;
}