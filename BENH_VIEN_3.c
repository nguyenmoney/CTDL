#include <stdio.h>
#include <stdlib.h> // Cho free() va EXIT_SUCCESS, EXIT_FAILURE

// Bao gom tat ca cac file header cua cac module can thiet
#include "structs.h"
#include "csv_handler.h" // Cho cac tien ich chung (neu dung)
#include "patient.h"
#include "department.h"
#include "doctor.h"
#include "medicine.h"
#include "examination.h"
#include "prescription.h"
#include "priority_queue.h"
#include "ui.h"

// --- Dinh nghia ten cac file CSV ---
// (Cac ten file nay da duoc dinh nghia trong header cua tung module,
// nhung de o day de de quan ly tap trung neu can thay doi duong dan)
// Tuy nhien, de nhat quan, nen su dung cac define tu header cua module tuong ung.
// Vi du: FILE_BENH_NHAN tu patient.h
// De tranh dinh nghia lai, chung ta se khong dinh nghia lai o day ma se
// truyen truc tiep cac define do vao ham xuLyLuaChonMenuChinh.


int main() {
    // --- Khoi tao cac con tro du lieu ---
    BenhNhan* dsBenhNhan = NULL;
    int soLuongBenhNhan = 0;

    KhoaPhong* dsKhoaPhong = NULL;
    int soLuongKhoaPhong = 0;

    BacSi* dsBacSi = NULL;
    int soLuongBacSi = 0;

    Thuoc* dsThuoc = NULL;
    int soLuongThuoc = 0;

    LanKham* dsLanKham = NULL;
    int soLuongLanKham = 0;

    DonThuocChiTiet* dsDonThuocChiTiet = NULL;
    int soLuongDTCT = 0;

    HangDoiUuTien hangDoiKhamBenh;
    khoiTaoHangDoi(&hangDoiKhamBenh); // Tu priority_queue.c

    // --- Tai du lieu tu cac file CSV ---
    // Thu tu tai du lieu co the quan trong neu co rang buoc khoa ngoai nghiem ngat
    // Hien tai, cac module kiem tra tinh hop le cua ma khoa ngoai khi nhap/sua.
    printf("Dang tai du lieu he thong...\n");

    // 1. Tai Khoa Phong
    dsKhoaPhong = docDanhSachKhoaPhong(FILE_KHOA_PHONG, &soLuongKhoaPhong);
    if (dsKhoaPhong == NULL && soLuongKhoaPhong == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Khoa Phong rong.\n", FILE_KHOA_PHONG);
        // Khong can thiet phai thoat, chuong trinh co the tao file moi khi ghi.
    }
    else if (dsKhoaPhong != NULL) {
        printf("Tai thanh cong %d Khoa Phong tu %s\n", soLuongKhoaPhong, FILE_KHOA_PHONG);
    }

    // 2. Tai Bac Si
    dsBacSi = docDanhSachBacSi(FILE_BAC_SI, &soLuongBacSi);
    if (dsBacSi == NULL && soLuongBacSi == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Bac Si rong.\n", FILE_BAC_SI);
    }
    else if (dsBacSi != NULL) {
        printf("Tai thanh cong %d Bac Si tu %s\n", soLuongBacSi, FILE_BAC_SI);
    }

    // 3. Tai Thuoc
    dsThuoc = docDanhSachThuoc(FILE_THUOC, &soLuongThuoc);
    if (dsThuoc == NULL && soLuongThuoc == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Thuoc rong.\n", FILE_THUOC);
    }
    else if (dsThuoc != NULL) {
        printf("Tai thanh cong %d loai Thuoc tu %s\n", soLuongThuoc, FILE_THUOC);
    }

    // 4. Tai Benh Nhan
    dsBenhNhan = docDanhSachBenhNhan(FILE_BENH_NHAN, &soLuongBenhNhan);
    if (dsBenhNhan == NULL && soLuongBenhNhan == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Benh Nhan rong.\n", FILE_BENH_NHAN);
    }
    else if (dsBenhNhan != NULL) {
        printf("Tai thanh cong %d Benh Nhan tu %s\n", soLuongBenhNhan, FILE_BENH_NHAN);
    }

    // 5. Tai Lan Kham
    dsLanKham = docDanhSachLanKham(FILE_LAN_KHAM, &soLuongLanKham);
    if (dsLanKham == NULL && soLuongLanKham == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Lan Kham rong.\n", FILE_LAN_KHAM);
    }
    else if (dsLanKham != NULL) {
        printf("Tai thanh cong %d Lan Kham tu %s\n", soLuongLanKham, FILE_LAN_KHAM);
    }

    // 6. Tai Chi Tiet Don Thuoc
    dsDonThuocChiTiet = docDanhSachDonThuocChiTiet(FILE_DON_THUOC_CHI_TIET, &soLuongDTCT);
    if (dsDonThuocChiTiet == NULL && soLuongDTCT == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Chi Tiet Don Thuoc rong.\n", FILE_DON_THUOC_CHI_TIET);
    }
    else if (dsDonThuocChiTiet != NULL) {
        printf("Tai thanh cong %d dong Chi Tiet Don Thuoc tu %s\n", soLuongDTCT, FILE_DON_THUOC_CHI_TIET);
    }

    printf("Hoan tat tai du lieu.\n");
    dungManHinhCho(); // Tu ui.c

    // --- Nap cac LanKham dang cho vao HangDoiUuTien ---
    if (dsLanKham != NULL) {
        for (int i = 0; i < soLuongLanKham; ++i) {
            if (dsLanKham[i].trangThai == LK_DANG_CHO) {
                if (!themVaoHangDoi(&hangDoiKhamBenh, dsLanKham[i])) {
                    fprintf(stderr, "Loi: Khong the them Lan Kham %d vao hang doi khi khoi tao.\n", dsLanKham[i].maLanKham);
                }
            }
        }
        printf("Da nap %d benh nhan vao hang doi cho kham.\n", hangDoiKhamBenh.soLuongHienTai);
        dungManHinhCho();
    }


    // --- Vong lap chinh cua chuong trinh ---
    // Bien choice duoc quan ly trong ham xuLyLuaChonMenuChinh (hoac co the co mot bien o day neu can)
    // Hien tai, xuLyLuaChonMenuChinh se loop cho den khi user chon thoat (tra ve 0).
    // Tuy nhien, cach thiet ke UI cua toi la xuLyLuaChonMenuChinh chi xu ly 1 lan chon,
    // vong lap se o main.
  // File: src/main.c - SAU KHI SUA (Trich doan vong lap chinh)
// ... (phan khoi tao va tai du lieu) ...
    int luaChonMenuChinh;
    do {
        hienThiMenuChinh(); // Tu ui.c

        printf("Vui long nhap lua chon cua ban (tu menu chinh): ");
        char bufferLuaChon[10];
        if (docDongAnToan(bufferLuaChon, sizeof(bufferLuaChon), stdin) != NULL) {
            if (sscanf(bufferLuaChon, "%d", &luaChonMenuChinh) != 1) {
                luaChonMenuChinh = -1; // Lua chon khong hop le
            }
        }
        else {
            luaChonMenuChinh = 0; // EOF hoac loi doc, coi nhu thoat
        }

        if (luaChonMenuChinh >= 1 && luaChonMenuChinh <= 10) { // Gia su co 10 chuc nang chinh
            xuLyLuaChonMenuChinh(
                luaChonMenuChinh, // <<<< TRUYEN LUA CHON DA DOC DUOC VAO DAY
                &dsBenhNhan, &soLuongBenhNhan, FILE_BENH_NHAN,
                &dsLanKham, &soLuongLanKham, FILE_LAN_KHAM,
                &dsKhoaPhong, &soLuongKhoaPhong, FILE_KHOA_PHONG,
                &dsBacSi, &soLuongBacSi, FILE_BAC_SI,
                &dsThuoc, &soLuongThuoc, FILE_THUOC,
                &dsDonThuocChiTiet, &soLuongDTCT, FILE_DON_THUOC_CHI_TIET,
                &hangDoiKhamBenh
            );
        }
        else if (luaChonMenuChinh == 0) {
            printf("Dang thoat chuong trinh...\n");
        }
        else { // luaChonMenuChinh la -1 (nhap khong phai so) hoac ngoai pham vi menu
            inThongDiepLoi("Lua chon khong hop le tu menu chinh. Vui long thu lai.");
            dungManHinhCho();
        }
    } while (luaChonMenuChinh != 0);



    // --- Giai phong bo nho ---
    printf("\nDang giai phong bo nho...\n");
    free(dsBenhNhan);
    free(dsKhoaPhong);
    free(dsBacSi);
    free(dsThuoc);
    free(dsLanKham);
    free(dsDonThuocChiTiet);
    giaiPhongHangDoi(&hangDoiKhamBenh); // Tu priority_queue.c
    printf("Da giai phong bo nho. Chuong trinh ket thuc.\n");

    return EXIT_SUCCESS; // Hoac 0
}