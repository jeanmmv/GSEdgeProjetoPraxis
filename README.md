# 🧠 Fluxo Mentor – Edge Computing (ESP32 + FIWARE)

## 👥 **Integrantes**
| Nome | RM |
|------|------|
| **Jean Carlos Rodrigues** | RM566439 |
| **Enrico Gianni Nóbrega Puttini** | RM561400 |

---

## 💡 **Sobre o Projeto**
O **Fluxo Mentor** é um sistema IoT desenvolvido com **ESP32**, **sensor ultrassônico**, **LCD**, **LEDs**, **botões** e **buzzer**, que simula um assistente inteligente para operadores em ambientes logísticos.

O dispositivo:
- Detecta o operador (≤ 10 cm);
- Ativa uma *missão* a cada **15 segundos**;
- Exibe animações e gráficos no LCD;
- Pisca o LED azul para indicar que a IA está “pensando”;
- Recebe respostas via botões;
- Envia estado e distância ao **FIWARE** via **MQTT/Ultralight 2.0**.

---

## 🔧 **Hardware Utilizado**
- ESP32  
- Sensor Ultrassônico HC-SR04  
- LCD 16x2 I2C  
- LED Azul (IA pensando)  
- LED Verde (entendeu)  
- LED Vermelho (não entendeu)  
- Buzzer  
- Botão Azul / Botão Vermelho  

---

## 📡 **Integração FIWARE**
### **Tópico MQTT**
```
/TEF/lamp001/attrs
```

### **Atributos (Ultralight 2.0)**
```
i = instant (distância)
s = state (SK-103 / SK-NOT-UNDERSTOOD)
```

---

## ⚙️ **Funcionamento**
- Missões são ativadas automaticamente a cada **15s**  
- LED azul pisca durante o processamento  
- LCD exibe animação “pensando...”  
- LCD mostra gráfico de barras com a distância  
- Botão azul → `s|SK-103`  
- Botão vermelho → `s|SK-NOT-UNDERSTOOD`  
- Dados enviados ao FIWARE via MQTT  
- Missão só é ativada se o operador estiver **≤ 10 cm**

---

## ▶ **Simulação no Wokwi**
🔗 https://wokwi.com/projects/446815872629354497

---

## 📂 **Estrutura do Repositório**
```
📦 fluxo-mentor/
├── src/
│   └── esp32_fluxo_mentor.ino
├── README.md
└── assets/   (opcional)
```

---

## 🏁 **Conclusão**
O Fluxo Mentor demonstra o uso eficiente de **computação de borda**, combinando resposta física rápida com integração em nuvem FIWARE para aplicações de supervisão industrial e logística.

