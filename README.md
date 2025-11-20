# 📝 README.md – FLUXO MENTOR (ESP32 + FIWARE + MQTT)

<h1 align="center">🤖 Fluxo Mentor – Assistente Inteligente para Operadores de Logística</h1>  
<p align="center">  
  <strong>Global Solution • FIAP • Engenharia de Software</strong><br>
  Edge Computing • IoT • MQTT • FIWARE • Wokwi • ESP32  
</p>

---

## 🚀 Sobre o Projeto

O **Fluxo Mentor** é um sistema inteligente de apoio a operadores de logística — representados aqui pelo “Sérgio” — que auxilia no fluxo de atividades em tempo real, permitindo que o usuário confirme se entendeu (ou não) a tarefa através de botões físicos.

O sistema utiliza:

- **ESP32** como microcontrolador  
- **MQTT** para enviar dados  
- **FIWARE IoT Agent** para interpretar os atributos  
- **Orion Context Broker** para armazenar o contexto  
- **STH-Comet** para histórico  
- **Wokwi** como simulador  
- **LCD, LEDs e Buzzer** para feedback físico  
- **Sensor Ultrassônico** para detectar proximidade  

O objetivo é criar um fluxo automatizado onde, a cada **15 segundos**, surge uma nova missão.  
O LED azul acende somente quando o operador está a **menos de 10 cm** do equipamento, indicando:

> **“A IA está pensando / enviando uma instrução”**

O operador deve responder:

- **Botão Azul → Entendeu (SK-103)**  
- **Botão Vermelho → Não entendeu (SK-NOT-UNDERSTOOD)**  

Esses dados são enviados ao FIWARE via **Ultralight 2.0**.

---

## 👨‍💻 Integrantes do Projeto

| Nome | RM |
|------|------|
| Enrico Gianni Nobrega Puttini | 561400 |
| Jean Carlos Rodrigues da Silva | 566439 |

---

## 🧩 Funcionamento do Sistema

### 📡 1. Conexão Wi-Fi + MQTT

- LCD mostra: **“Conectando WiFi…”**
- Ao conectar: **“WiFi conectado!”**
- O ESP32 se conecta ao **broker MQTT / IoT Agent**.

---

### 🔵 2. Detecção de Proximidade (HC-SR04)

- IA **só dispara missão se o operador estiver ≤ 10 cm**
- A distância é exibida no LCD com um **gráfico de barras**

---

### ⏱ 3. Missão Automática a Cada 15s

Se o operador estiver próximo:

- LED **azul acende**
- LCD mostra **"NOVA MISSAO!"**
- Buzzer toca dois tons curtos
- LED azul **pisca** enquanto a IA “pensa”

---

### 🟢🔴 4. Resposta do Operador

O operador utiliza os botões:

- **Botão Azul** → `SK-103`
- **Botão Vermelho** → `SK-NOT-UNDERSTOOD`

Enviado ao FIWARE como:

```text
i|<distancia>
s|SK-103
```

ou

```text
s|SK-NOT-UNDERSTOOD
```

Enviado para:

- IoT Agent MQTT  
- Orion Context Broker  
- STH-Comet (histórico)

---

## 📡 Estrutura FIWARE / MQTT

### ➤ Tópico de envio (Ultralight 2.0)

```text
/TEF/lamp001/attrs
```

### ➤ Atributos configurados

| ID | Nome | Tipo |
|----|------|-------|
| `i` | instant (distância) | Float |
| `s` | state (resposta) | Text |

---

## 🔧 Hardware Utilizado

| Componente | Uso |
|------------|-----|
| ESP32 DevKit V1 | Microcontrolador |
| Sensor Ultrassônico HC-SR04 | Medição de distância |
| LCD 16x2 I2C | Interface de texto |
| Botão Azul | “Sérgio entendeu” |
| Botão Vermelho | “Sérgio não entendeu” |
| LED Azul | IA pensando |
| LED Verde | Resposta positiva |
| LED Vermelho | Resposta negativa |
| Buzzer | Feedback sonoro |

---

## 🛰 Arquitetura Geral

```text
[Sensor HC-SR04]──┐
                  ├──> [ESP32] ──MQTT──> [FIWARE IoT Agent]
[Botões / LEDs]───┘                 │
                                    ├──> [Orion Context Broker]
                                    └──> [STH-Comet / Histórico]
```

---

## 🧪 Funcionalidades Implementadas

✔ LED azul pisca quando a missão está ativa  
✔ Animação **“Pensando…”** no LCD  
✔ Gráfico de barras com a distância  
✔ Missão automática a cada **15 segundos**  
✔ Buzzer ao iniciar missão  
✔ Debounce nos botões  
✔ LED azul somente se o operador ≤ 10 cm  
✔ Envio ao FIWARE (Ultralight 2.0)  
✔ Logs completos no Serial Monitor  
✔ Feedback completo via LEDs e LCD  

---

## 🖥 Como Rodar o Projeto no Wokwi

1. Acesse o projeto no simulador  
2. Cole o código do ESP32  
3. Inicie a simulação  
4. Use os botões (Azul/Vermelho)  
5. Veja os logs no console Serial  
6. Veja as atualizações no FIWARE  

🔗 **Wokwi:**  
https://wokwi.com/projects/447906452922479617

---

## 🎥 Demonstração em Vídeo

📺 **YouTube:**  
https://www.youtube.com/watch?v=RXW5s48QxTU

---

## 📎 Repositório Oficial  

https://github.com/jeanmmv/GSEdgeProjetoPraxis

---

## 🎓 FIAP • Engenharia de Software • 2025  

Projeto desenvolvido como parte da **Global Solution – Edge Computing & IoT**.
