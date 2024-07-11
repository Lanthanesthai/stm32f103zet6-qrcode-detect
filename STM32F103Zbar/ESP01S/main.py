import sys
import requests
import time
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QLabel, QPushButton, QTextEdit, QCheckBox)
from PyQt5.QtCore import QThread, pyqtSignal


class DataFetcher(QThread):
    status_update = pyqtSignal(str)
    data_update = pyqtSignal(str)

    def __init__(self, url):
        super().__init__()
        self.url = url
        self.running = True

    def run(self):
        while self.running:
            try:
                response = requests.get(self.url)
                if response.status_code == 200:
                    self.status_update.emit("连接成功")
                    data = response.text.strip()
                    if data:
                        self.data_update.emit(f"检测结果为：{data}")
                    else:
                        self.data_update.emit("无结果")
                else:
                    self.status_update.emit("无结果")
            except requests.ConnectionError:
                self.status_update.emit("连接失败")
            time.sleep(2)

    def stop(self):
        self.running = False


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.initUI()
        self.data_fetcher = DataFetcher("http://192.168.4.1/")
        self.data_fetcher.status_update.connect(self.update_status)
        self.data_fetcher.data_update.connect(self.update_data)
        self.data_fetcher.start()

    def initUI(self):
        self.setWindowTitle("数据获取")

        self.layout = QVBoxLayout()

        self.status_label = QLabel("连接中……")
        self.layout.addWidget(self.status_label)

        self.data_check = QCheckBox("显示数据")
        self.data_check.stateChanged.connect(self.toggle_data_display)
        self.layout.addWidget(self.data_check)

        self.data_display = QTextEdit()
        self.data_display.setReadOnly(True)
        self.data_display.setVisible(False)
        self.layout.addWidget(self.data_display)

        self.export_button = QPushButton("导出数据")
        self.export_button.clicked.connect(self.export_data)
        self.layout.addWidget(self.export_button)

        self.setLayout(self.layout)

    def update_status(self, status):
        self.status_label.setText(status)

    def update_data(self, data):
        if self.data_check.isChecked():
            self.data_display.append(data)

    def toggle_data_display(self, state):
        self.data_display.setVisible(state)

    def export_data(self):
        with open("data_output.txt", "w") as file:
            file.write(self.data_display.toPlainText())

    def closeEvent(self, event):
        self.data_fetcher.stop()
        self.data_fetcher.wait()
        event.accept()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())
