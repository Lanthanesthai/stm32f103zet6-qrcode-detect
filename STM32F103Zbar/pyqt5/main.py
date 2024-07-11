import sys
import serial
import serial.tools.list_ports
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QComboBox,
                             QPushButton, QTextEdit, QLineEdit, QCheckBox)
from PyQt5.QtGui import QFont, QTextCursor
from PyQt5.QtCore import QTimer


class SerialCommunicator(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()
        self.serial = serial.Serial()
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.receive_data)

    def initUI(self):
        self.setWindowTitle('串口通信')

        # Set font
        font = QFont('SimSun', 12)  # SimSun is a common Chinese font

        # Layout
        layout = QVBoxLayout()

        # Serial Port Selection
        port_layout = QHBoxLayout()
        self.port_label = QLabel('端口:')
        self.port_label.setFont(font)
        self.port_combo = QComboBox()
        self.port_combo.setFont(font)
        self.refresh_ports()
        port_layout.addWidget(self.port_label)
        port_layout.addWidget(self.port_combo)

        # Baudrate Selection
        baudrate_layout = QHBoxLayout()
        self.baudrate_label = QLabel('波特率:')
        self.baudrate_label.setFont(font)
        self.baudrate_combo = QComboBox()
        self.baudrate_combo.setFont(font)
        self.baudrate_combo.addItems(['9600', '115200', '57600', '38400', '19200', '4800'])
        baudrate_layout.addWidget(self.baudrate_label)
        baudrate_layout.addWidget(self.baudrate_combo)

        # Decode Mode Selection
        decode_layout = QHBoxLayout()
        self.decode_label = QLabel('解码模式:')
        self.decode_label.setFont(font)
        self.decode_combo = QComboBox()
        self.decode_combo.setFont(font)
        self.decode_combo.addItems(['HEX', 'UTF-8', 'GBK'])
        decode_layout.addWidget(self.decode_label)
        decode_layout.addWidget(self.decode_combo)

        # Open/Close Button
        self.open_button = QPushButton('打开端口')
        self.open_button.setFont(font)
        self.open_button.clicked.connect(self.toggle_port)

        # Send Area
        self.send_text = QLineEdit()
        self.send_text.setFont(font)
        self.send_button = QPushButton('发送')
        self.send_button.setFont(font)
        self.send_button.clicked.connect(self.send_data)

        # Receive Area
        self.receive_text = QTextEdit()
        self.receive_text.setFont(font)
        self.receive_text.setReadOnly(True)

        # Auto-scroll option
        self.auto_scroll = QCheckBox("自动滚动")
        self.auto_scroll.setFont(font)
        self.auto_scroll.setChecked(True)

        # Add widgets to layout
        layout.addLayout(port_layout)
        layout.addLayout(baudrate_layout)
        layout.addLayout(decode_layout)
        layout.addWidget(self.open_button)
        layout.addWidget(self.send_text)
        layout.addWidget(self.send_button)
        layout.addWidget(self.receive_text)
        layout.addWidget(self.auto_scroll)

        self.setLayout(layout)

    def refresh_ports(self):
        ports = serial.tools.list_ports.comports()
        self.port_combo.clear()
        for port in ports:
            self.port_combo.addItem(port.device)

    def toggle_port(self):
        if self.serial.is_open:
            self.serial.close()
            self.open_button.setText('打开端口')
            self.timer.stop()  # Stop the timer when port is closed
        else:
            self.serial.port = self.port_combo.currentText()
            self.serial.baudrate = int(self.baudrate_combo.currentText())
            try:
                self.serial.open()
                self.open_button.setText('关闭端口')
                self.timer.start(100)  # Start the timer with 100ms interval to check for incoming data
            except Exception as e:
                self.receive_text.append(f"无法打开端口: {e}")

    def send_data(self):
        if self.serial.is_open:
            data = self.send_text.text()
            if self.decode_combo.currentText() == 'HEX':
                data = bytes.fromhex(data)
            else:
                encoding = self.decode_combo.currentText().lower()
                data = data.encode(encoding)
            self.serial.write(data)
            self.send_text.clear()
        else:
            self.receive_text.append("端口未打开!")

    def receive_data(self):
        if self.serial.is_open:
            while self.serial.in_waiting:
                data = self.serial.read_all()
                if self.decode_combo.currentText() == 'HEX':
                    data = data.hex()
                else:
                    encoding = self.decode_combo.currentText().lower()
                    data = data.decode(encoding, errors='ignore')
                self.receive_text.append(data)
                if self.auto_scroll.isChecked():
                    self.receive_text.moveCursor(QTextCursor.End)

    def closeEvent(self, event):
        if self.serial.is_open:
            self.serial.close()
        event.accept()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = SerialCommunicator()
    window.show()
    sys.exit(app.exec_())
