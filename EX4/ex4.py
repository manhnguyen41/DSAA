import json
from datetime import datetime, date
from datetime import timedelta

tien1 = [130000, 120000, 110000, 100000, 90000, 80000]
tien2 = [200000, 180000, 160000, 140000, 120000, 120000]
tien3 = [180000, 160000, 140000, 120000, 110000, 100000]

def readFromFile(fileName):
    with open(fileName, "r") as f:
        content = json.loads(f.read())
        f.close()
        return content

def writeToFile(fileName, content):
    with open(fileName, "w") as f:
        f.write(json.dumps(content))

def ProcessReadFromFile():
    print("Nhập tên file cần đọc dữ liệu: ")
    fileName = input()
    content = readFromFile(fileName)
    return content

def ProcessWriteToFile(content):
    print("Nhập tên file cần ghi dữ liệu:")
    fileName = input()
    writeToFile(fileName, content)

def printDict(item):
    for key, value in item.items():
        print(key, ': ', value)

def ProcessFindMoto(motos):
    print("Nhập tên xe cần tìm: ")
    motoName = input()
    for moto in motos:
        if (moto["Loai xe"] == motoName):
            printDict(moto)

def ProcessFindContact(contacts):
    print("Nhập tên người cần tìm: ")
    name = input()
    for contact in contacts:
        if (contact["Ten nguoi"] == name):
            printDict(contact)

def convertTime(t):
    if (len(t) <= 10):
        t += " 00:00:00" 
    year = int(t[0:4])
    month = int(t[5:7])
    day = int(t[8:10])
    hour = int(t[11:13])
    minute = int(t[14:16])
    second = int(t[17:19])
    return datetime(year = year, month = month, day = day, hour = hour, minute = minute, second = second)

def lamTronTheoNgay(t):
    s = t.total_seconds()
    s1 = s // 24 // 3600
    if (s % 24 % 3600 > 0):
        s1 = s1 + 1
    return s1

def lamTronTheoGio(t):
    s = t.total_seconds()
    s1 = s // 3600
    if (s % 3600 > 0):
        s1 = s1 + 1
    return s1

def tinhTien1(d, tien):
    soTien = 0
    if (d <= 5):
        soTien = soTien + tien[0] * d
    elif (d <= 10):
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + d * tien[1]
    elif (d <= 15):
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + 5 * tien[1]
        d = d - 5
        soTien = soTien + d * tien[2]
    elif (d <= 20):
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + 5 * tien[1]
        d = d - 5
        soTien = soTien + 5 * tien[2]
        d = d - 5
        soTien = soTien + d * tien[3]
    elif (d <= 25):
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + 5 * tien[1]
        d = d - 5
        soTien = soTien + 5 * tien[2]
        d = d - 5
        soTien = soTien + 5 * tien[3]
        d = d - 5
        soTien = soTien + d * tien[4]
    elif (d <= 30):
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + 5 * tien[1]
        d = d - 5
        soTien = soTien + 5 * tien[2]
        d = d - 5
        soTien = soTien + 5 * tien[3]
        d = d - 5
        soTien = soTien + 5 * tien[4]
        d = d - 5
        soTien = soTien + d * tien[5]
    else:
        soTien = soTien + tien[0] * 5
        d = d - 5
        soTien = soTien + 5 * tien[1]
        d = d - 5
        soTien = soTien + 5 * tien[2]
        d = d - 5
        soTien = soTien + 5 * tien[3]
        d = d - 5
        soTien = soTien + 5 * tien[4]
        d = d - 5
        soTien = soTien + 5 * tien[5]
        d = d - 5
        soTien = soTien + d * tien[5]
    return soTien

def tinhTien(motos, contacts):
    for contact in contacts:
        t1 = convertTime(contact["Thoi diem thue"])
        t2 = convertTime(contact["Thoi diem tra"])
        t3 = t2 - t1
        soTien = 0
        if (lamTronTheoGio(t3) < 24):
            for motoName in contact["Loai xe"]:
                soTien += tinhTienTheoGio(lamTronTheoGio(t3), motos, motoName)
        else:
            for motoName in contact["Loai xe"]:
                if (motoName == "Yamaha Sirius 110cc"):
                    soTien = soTien + tinhTien1(lamTronTheoNgay(t3), tien1)
                elif (motoName == "Honda Air Blade 125cc"):
                    soTien = soTien + tinhTien1(lamTronTheoNgay(t3), tien2)
                else:
                    soTien = soTien + tinhTien1(lamTronTheoNgay(t3), tien3)
        contact["Tong tien thanh toan"] = soTien
    return contacts

def compare(t1, t2):
    t3 = (t1 - t2).total_seconds()
    if (t3 < 0):
        return -1
    elif (t3 == 0):
        return 0
    else:
        return 1

def tinhTienTheoGio(h, motos, motoName):
    soTien = 0
    for moto in motos:
        if (moto["Loai xe"] == motoName):
            if (moto["Model"] == "Xe so"):
                soTien += 50000 * h
            else:
                soTien += 80000 * h
    return soTien

def demNgay(t3, t4):
    t = datetime(t3.year, t3.month, t3.day)
    dem = 0
    while (compare(t, t4) == -1):
        t += timedelta(1)
        dem += 1
    return dem

def tinhTienTrongNgay(t, motos, contacts):
    soTien = 0
    for contact in contacts:
        t1 = convertTime(contact["Thoi diem thue"])
        t2 = convertTime(contact["Thoi diem tra"])
        t3 = t2 - t1
        if (lamTronTheoGio(t3) < 24):
            if (compare(t1, t) == -1 and compare(t2, t) == 1):
                h = lamTronTheoGio(t2 - t)
                for motoName in contact["Loai xe"]:
                    soTien += tinhTienTheoGio(h, motos, motoName)
            elif (compare(t1, t) == 1):
                t4 = t + timedelta(1)
                if (compare(t2, t4) == 1):
                    h = lamTronTheoGio(t4 - t1)
                    for motoName in contact["Loai xe"]:
                        soTien += tinhTienTheoGio(h, motos, motoName)
                else:
                    h = lamTronTheoGio(t2 - t1)
                    for motoName in contact["Loai xe"]:
                        soTien += tinhTienTheoGio(h, motos, motoName)
        else:
            if ((compare(t1, t) == -1 and compare(t2, t) == 1) or (compare(t1, t + timedelta(1)) == -1 and compare(t1, t) == 1)):
                soTien += contact["Tong tien thanh toan"] / demNgay(t1, t2)
    return soTien

def tinhTienTrongThang(m, y, motos, contacts):
    t = datetime(y, m, 1)
    soTien = 0
    while (t.month == m):
        soTien += tinhTienTrongNgay(t, motos, contacts)
        t += timedelta(1)
    return soTien

def ProcessBaoCaoDoanhThuTheoNgay(motos, contacts):
    print("Nhập ngày (yyyy/mm/dd): ")
    t = input()
    t1 = datetime(int(t[0:4]), int(t[5:7]), int(t[8:10]))
    print("Doanh thu trong ngày ", t, " là: ", tinhTienTrongNgay(t1, motos, contacts))

def ProcessBaoCaoDoanhThuTheoThang(motos, contacts):
    print("Nhập tháng (yyyy/mm): ")
    t = input()
    print("Doanh thu trong tháng ", t, " là: ", tinhTienTrongThang(int(t[5:7]), int(t[0:4]), motos, contacts))

if __name__ == "__main__":
    content = {}
    motos = []
    contacts = []
    while (1):
        print("1.Đọc dữ liệu từ file.")
        print("2.Ghi dữ liệu vào file.")
        print("3.Tìm kiếm xe.")
        print("4.Tìm kiếm hợp đồng.")
        print("5.Tính tiền.")
        print("6.Báo cáo doanh thu theo ngày.")
        print("7.Báo cáo doanh thu theo tháng.")
        print("8.Thoát.")
        print("Vui lòng chọn chức năng:") 
        cmd = int(input())
        if (cmd == 1):
            content = ProcessReadFromFile()
            motos = content["motos"]
            contacts = content["contacts"]
        elif (cmd == 2):
            content["motos"] = motos
            content["contacts"] = contacts
            ProcessWriteToFile(content)
        elif (cmd == 3):
            ProcessFindMoto(motos)
        elif (cmd == 4):
            ProcessFindContact(contacts)
        elif (cmd == 5):
            contacts = tinhTien(motos, contacts)
        elif (cmd == 6):
            ProcessBaoCaoDoanhThuTheoNgay(motos, contacts)
        elif (cmd == 7):
            ProcessBaoCaoDoanhThuTheoThang(motos, contacts)
        else:
            break