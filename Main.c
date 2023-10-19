#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RTClib.h>
#include <Wire.h>

RTC_DS3231 rtc;
Adafruit_ILI9341 tft = Adafruit_ILI9341(10, 9);

int portaParaAcionar = 13;
bool luzExterna = false;
int temperaturaDesejada = 25;
bool arCondicionadoLigado = false;

// Variáveis para equipamentos adicionais
bool equipamento1Ligado = false;
int equipamento1HoraLigar = 8;  // Hora para ligar (no formato 24 horas)
int equipamento1HoraDesligar = 18;  // Hora para desligar (no formato 24 horas)
int equipamento1MinutosLigar = 0;  // Minutos para ligar
int equipamento1MinutosDesligar = 30;  // Minutos para desligar

bool equipamento2Ligado = false;
int equipamento2HoraLigar = 10;
int equipamento2HoraDesligar = 16;
int equipamento2MinutosLigar = 15;
int equipamento2MinutosDesligar = 45;

bool equipamento3Ligado = false;
int equipamento3HoraLigar = 12;
int equipamento3HoraDesligar = 14;
int equipamento3MinutosLigar = 0;
int equipamento3MinutosDesligar = 0;

bool equipamento4Ligado = false;
int equipamento4HoraLigar = 17;
int equipamento4HoraDesligar = 20;
int equipamento4MinutosLigar = 0;
int equipamento4MinutosDesligar = 0;

void setup() {
  pinMode(portaParaAcionar, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  Wire.begin();
  rtc.begin();
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
}

void loop() {
  DateTime now = rtc.now();
  String horarioDesejado = "12:30";
  
  acionarPortaNoHorario(horarioDesejado);
  controlarIluminacaoExterna();
  controlarArCondicionado();

  // Funções para controlar equipamentos adicionais
  controlarEquipamento(equipamento1Ligado, equipamento1HoraLigar, equipamento1MinutosLigar, equipamento1HoraDesligar, equipamento1MinutosDesligar);
  controlarEquipamento(equipamento2Ligado, equipamento2HoraLigar, equipamento2MinutosLigar, equipamento2HoraDesligar, equipamento2MinutosDesligar);
  controlarEquipamento(equipamento3Ligado, equipamento3HoraLigar, equipamento3MinutosLigar, equipamento3HoraDesligar, equipamento3MinutosDesligar);
  controlarEquipamento(equipamento4Ligado, equipamento4HoraLigar, equipamento4MinutosLigar, equipamento4HoraDesligar, equipamento4MinutosDesligar);
}

int obterHoraDesejada(String horario) {
  return horario.substring(0, 2).toInt();
}

int obterMinutoDesejado(String horario) {
  return horario.substring(3).toInt();
}

void atualizarDisplay(String mensagem) {
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(10, 10);
  tft.print("Ciclos Realizados: ");
  tft.setCursor(10, 40);
  tft.print(mensagem);
}

void acionarPortaNoHorario(String horarioDesejado) {
  DateTime now = rtc.now();
  if (now.hour() == obterHoraDesejada(horarioDesejado) && now.minute() == obterMinutoDesejado(horarioDesejado)) {
    digitalWrite(portaParaAcionar, HIGH);
    atualizarDisplay("Porta Acionada");
  } else {
    digitalWrite(portaParaAcionar, LOW);
    atualizarDisplay("Porta Desligada");
  }
}

void controlarIluminacaoExterna() {
  int luzExternaSensor = digitalRead(12);
  if (luzExternaSensor == HIGH) {
    luzExterna = true;
  } else {
    luzExterna = false;
  }
}

void controlarArCondicionado() {
  float temperaturaAtual = lerTemperatura();
  if (temperaturaAtual > temperaturaDesejada && !arCondicionadoLigado) {
    arCondicionadoLigado = true;
  } else if (temperaturaAtual <= temperaturaDesejada && arCondicionadoLigado) {
    arCondicionadoLigado = false;
  }
}

float lerTemperatura() {
  // Simulação de leitura de temperatura
  return 25.5;
}

// Função para controlar equipamentos adicionais com base no horário
void controlarEquipamento(bool &equipamentoLigado, int horaLigar, int minutosLigar, int horaDesligar, int minutosDesligar) {
  DateTime now = rtc.now();
  int horaAtual = now.hour();
  int minutosAtuais = now.minute();
  
  // Verifica se o equipamento deve ser ligado
  if (horaAtual == horaLigar && minutosAtuais >= minutosLigar) {
    equipamentoLigado = true;
  } else if (horaAtual > horaLigar) {
    equipamentoLigado = true;
  }
  
  // Verifica se o equipamento deve ser desligado
  if (horaAtual == horaDesligar && minutosAtuais >= minutosDesligar) {
    equipamentoLigado = false;
  } else if (horaAtual > horaDesligar) {
    equipamentoLigado = false;
  }
}
