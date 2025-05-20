#include "ui.h"
#include "patient.h"        // Cho cac ham xu ly Benh Nhan, timBenhNhanTheoMaChuoi, gioiTinhToString, stringToGioiTinh
#include "examination.h"    // Cho cac ham xu ly Lan Kham, enums, mucDoUuTienToString, capNhatThongTinLanKham, dangKyKhamMoi, vv
#include "department.h"   // Cho cac ham xu ly Khoa Phong, timKhoaPhongTheoMa, nhapThongTinKhoaPhongTuBanPhim, themKhoaPhong, suaThongTinKhoaPhong, xoaKhoaPhong, inDanhSachKhoaPhongConsole
#include "doctor.h"       // Cho cac ham xu ly Bac Si, timBacSiTheoMa, nhapThongTinBacSiTuBanPhim, themBacSi, suaThongTinBacSi, xoaBacSi, inDanhSachBacSiConsole
#include "medicine.h"     // Cho cac ham xu ly Thuoc, timThuocTheoMa, nhapThongTinThuocTuBanPhim, themThuoc, suaThongTinThuoc, xoaThuoc, inDanhSachThuocConsole
#include "prescription.h" // Cho cac ham xu ly Don Thuoc, quanLyCapNhatChiTietDonThuoc
#include "priority_queue.h"// Cho cac ham xu ly Hang Doi
#include "csv_handler.h"  // Cho docDongAnToan
#include "structs.h"      // Cho cac MAX_LEN... defines va cac struct

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>  
#include <time.h>   
#include <math.h>   // Cho difftime (va fmod neu can)

// --- Dinh nghia cac ham tien ich giao dien ---

void xoaManHinhConsole() {
#ifdef _WIN32
    system("cls");
#else
    
    int result = system("clear");
    if (result != 0) { 
        for (int i = 0; i < 50; ++i) printf("\n");
    }
#endif
}

// Phien ban dungManHinhCho da duoc sua de chi can 1 Enter
void dungManHinhCho() {
    printf("\nNhan Enter de tiep tuc...");
    int c;
    // Tieu thu bat ky ky tu nao con lai trong bo dem input,
    // bao gom ca newline dau tien neu co tu lenh nhap truoc do.
    // Neu buffer rong (vi du sau docDongAnToan), getchar() dau tien se cho.
    while ((c = getchar()) != '\n' && c != EOF);
    // Ham ket thuc sau khi newline (hoac EOF) da duoc tieu thu.
    // Khong can them getchar() nua.
}

int nhapSoNguyenCoGioiHan(const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s (%d-%d): ", prompt, min, max);
        if (docDongAnToan(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
                return value;
            }
        }
        else {
            printf("\nKet thuc nhap lieu hoac co loi. Quay lai menu.\n");
            dungManHinhCho(); // Cho nguoi dung doc thong bao
            return min - 1; // Tra ve gia tri khong hop le de bao hieu thoat/loi
        }
        inThongDiepLoi("Lua chon khong hop le. Vui long nhap lai.");
    }
}

int nhapSoNguyenKhongAm(const char* prompt) {
    int value;
    char buffer[100];
    printf("%s: ", prompt);
    if (docDongAnToan(buffer, sizeof(buffer), stdin) != NULL) {
        if (sscanf(buffer, "%d", &value) == 1 && value >= 0) {
            return value;
        }
    }
    else {
        printf("\nKet thuc nhap lieu hoac co loi.\n");
        dungManHinhCho();
        return -2; // Gia tri dac biet bao loi doc/EOF
    }
    inThongDiepLoi("So nhap vao khong hop le hoac khong phai so nguyen khong am.");
    return -1;
}


void inThongDiepLoi(const char* message) {
    printf("LOI: %s\n", message);
}

void inThongDiepThanhCong(const char* message) {
    printf("THANH CONG: %s\n", message);
}

// --- Dinh nghia cac ham menu con ---
// (Noi dung cac ham menu con nhu menuQuanLyBenhNhan, menuQuanLyLanKham, 
// menuQuanLyKhoaPhong, menuQuanLyBacSi, menuQuanLyThuoc
// se duoc lay tu phien ban ban da cung cap, voi cac chinh sua da duoc thuc hien
// cho chuc nang sua co chon loc va cac loi nho khac)

// Ham menuQuanLyBenhNhan (da bao gom chuc nang sua co chon loc)
void menuQuanLyBenhNhan(BenhNhan** dsBenhNhan, int* soLuongBenhNhan, const char* tenFileCSV) {
    // ... (Noi dung ham menuQuanLyBenhNhan nhu trong phien ban ui_c_final_corrected_may16_v3) ...
    // ... (Dam bao da #include "patient.h" o dau file ui.c) ...
    int choice;
    do {
        xoaManHinhConsole();
        printf("--- QUAN LY BENH NHAN ---\n");
        printf("1. Them benh nhan moi\n");
        printf("2. Sua thong tin benh nhan\n");
        printf("3. Xoa benh nhan\n");
        printf("4. Xem danh sach benh nhan\n");
        printf("5. Tim kiem benh nhan\n");
        printf("0. Quay lai menu chinh\n");
        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 5);
        if (choice < 0) { choice = 0; }

        switch (choice) {
        case 1: {
            BenhNhan bnMoi;
            printf("\n--- THEM BENH NHAN MOI ---\n");
            if (nhapThongTinBenhNhanTuBanPhim(&bnMoi, *dsBenhNhan, *soLuongBenhNhan)) {
                if (themBenhNhan(dsBenhNhan, soLuongBenhNhan, bnMoi, tenFileCSV)) {
                    inThongDiepThanhCong("Them benh nhan moi thanh cong!");
                }
                else {
                    inThongDiepLoi("Them benh nhan moi that bai.");
                }
            }
            else {
                inThongDiepLoi("Huy them benh nhan moi hoac thong tin nhap khong hop le/bi trung.");
            }
            dungManHinhCho();
            break;
        }
        case 2: {
            char maSua_str[MAX_LEN_MA_DINH_DANH_BN];
            printf("Nhap Ma Dinh Danh cua benh nhan can sua: ");
            if (docDongAnToan(maSua_str, sizeof(maSua_str), stdin) == NULL || strlen(maSua_str) == 0) {
                inThongDiepLoi("Ma dinh danh khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int k = 0; maSua_str[k]; k++) maSua_str[k] = toupper(maSua_str[k]);

            int benhNhanIndex = -1;
            for (int i = 0; i < *soLuongBenhNhan; ++i) {
                if (strcmp((*dsBenhNhan)[i].maDinhDanh, maSua_str) == 0) {
                    benhNhanIndex = i;
                    break;
                }
            }

            if (benhNhanIndex != -1) {
                BenhNhan bnTamSua = (*dsBenhNhan)[benhNhanIndex];
                int suaLuaChonNoiBo;
                char bufferNhapLieu[1000];

                do {
                    xoaManHinhConsole();
                    printf("--- SUA THONG TIN BENH NHAN (Ma DD: %s) ---\n", bnTamSua.maDinhDanh);
                    printf("Thong tin hien tai:\n");
                    printf("  1. Ten           : %s\n", bnTamSua.ten);
                    printf("  2. So Dien Thoai : %s\n", bnTamSua.soDienThoai);
                    printf("  3. BHYT          : %s\n", bnTamSua.bhyt);
                    char ngaySinhStr_ui[12];
                    snprintf(ngaySinhStr_ui, sizeof(ngaySinhStr_ui), "%02d/%02d/%04d", bnTamSua.ngaySinh.ngay, bnTamSua.ngaySinh.thang, bnTamSua.ngaySinh.nam);
                    printf("  4. Ngay Sinh     : %s\n", ngaySinhStr_ui);
                    printf("  5. Tuoi          : %d\n", bnTamSua.tuoi);
                    printf("  6. Gioi Tinh     : %s\n", gioiTinhToString(bnTamSua.gioiTinh));
                    printf("  7. Dia Chi       : %s\n", bnTamSua.diaChi);
                    printf("  8. Tieu Su Benh Ly: %s\n", bnTamSua.tieuSuBenhLy);
                    printf("-------------------------------------------------\n");
                    printf("Chon thong tin muon sua:\n");
                    printf("  1. Sua Ten\n  2. Sua So Dien Thoai\n  3. Sua BHYT\n  4. Sua Ngay Sinh\n  5. Sua Tuoi\n  6. Sua Gioi Tinh\n  7. Sua Dia Chi\n  8. Sua Tieu Su Benh Ly\n");
                    printf("-------------------------------------------------\n");
                    printf("  0. HOAN TAT SUA DOI VA LUU\n");
                    printf("  9. Huy bo tat ca thay doi va quay lai\n");

                    suaLuaChonNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
                    if (suaLuaChonNoiBo < 0) { suaLuaChonNoiBo = 9; }

                    switch (suaLuaChonNoiBo) {
                    case 1: printf("Nhap Ten moi (hien tai: %s): ", bnTamSua.ten); docDongAnToan(bnTamSua.ten, sizeof(bnTamSua.ten), stdin); break;
                    case 2: printf("Nhap So Dien Thoai moi (hien tai: %s): ", bnTamSua.soDienThoai); docDongAnToan(bnTamSua.soDienThoai, sizeof(bnTamSua.soDienThoai), stdin); break;
                    case 3: printf("Nhap BHYT moi (hien tai: %s): ", bnTamSua.bhyt); docDongAnToan(bnTamSua.bhyt, sizeof(bnTamSua.bhyt), stdin); break;
                    case 4:
                        printf("Nhap Ngay Sinh moi (ngay thang nam, vd: 15 01 1990) (hien tai: %s): ", ngaySinhStr_ui);
                        if (docDongAnToan(bufferNhapLieu, sizeof(bufferNhapLieu), stdin) != NULL && sscanf(bufferNhapLieu, "%d %d %d", &bnTamSua.ngaySinh.ngay, &bnTamSua.ngaySinh.thang, &bnTamSua.ngaySinh.nam) == 3) {}
                        else { inThongDiepLoi("Dinh dang Ngay Sinh khong hop le."); }
                        break;
                    case 5:
                        printf("Nhap Tuoi moi (hien tai: %d): ", bnTamSua.tuoi);
                        if (docDongAnToan(bufferNhapLieu, sizeof(bufferNhapLieu), stdin) != NULL && sscanf(bufferNhapLieu, "%d", &bnTamSua.tuoi) == 1 && bnTamSua.tuoi >= 0) {}
                        else { inThongDiepLoi("Tuoi khong hop le."); }
                        break;
                    case 6:
                        printf("Nhap Gioi Tinh moi (Nam, Nu, Khac) (hien tai: %s): ", gioiTinhToString(bnTamSua.gioiTinh));
                        docDongAnToan(bufferNhapLieu, sizeof(bufferNhapLieu), stdin);
                        bnTamSua.gioiTinh = stringToGioiTinh(bufferNhapLieu);
                        break;
                    case 7: printf("Nhap Dia Chi moi (hien tai: %s): ", bnTamSua.diaChi); docDongAnToan(bnTamSua.diaChi, sizeof(bnTamSua.diaChi), stdin); break;
                    case 8: printf("Nhap Tieu Su Benh Ly moi (hien tai: %s): ", bnTamSua.tieuSuBenhLy); docDongAnToan(bnTamSua.tieuSuBenhLy, sizeof(bnTamSua.tieuSuBenhLy), stdin); break;
                    case 0:
                        if (suaThongTinBenhNhan(*dsBenhNhan, *soLuongBenhNhan, maSua_str, bnTamSua, tenFileCSV)) {
                            inThongDiepThanhCong("Da luu cac thay doi cho benh nhan!");
                        }
                        else { inThongDiepLoi("Loi khi luu thay doi."); }
                        break;
                    case 9: inThongDiepLoi("Da huy bo sua doi."); suaLuaChonNoiBo = 0; break;
                    default: inThongDiepLoi("Lua chon khong hop le."); dungManHinhCho(); break;
                    }
                    if (suaLuaChonNoiBo >= 1 && suaLuaChonNoiBo <= 8) {
                        inThongDiepThanhCong("Da cap nhat truong. Chon truong khac hoac Luu/Huy.");
                        dungManHinhCho();
                    }
                } while (suaLuaChonNoiBo != 0);
            }
            else {
                inThongDiepLoi("Khong tim thay benh nhan voi ma dinh danh da nhap.");
            }
            dungManHinhCho();
            break;
        }
        case 3: {
            char maXoa_str[MAX_LEN_MA_DINH_DANH_BN];
            printf("Nhap Ma Dinh Danh cua benh nhan can xoa: ");
            if (docDongAnToan(maXoa_str, sizeof(maXoa_str), stdin) == NULL || strlen(maXoa_str) == 0) {
                inThongDiepLoi("Ma dinh danh khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int k = 0; maXoa_str[k]; k++) maXoa_str[k] = toupper(maXoa_str[k]);

            printf("Ban co chac chan muon xoa benh nhan co Ma DD: %s? (c/k): ", maXoa_str);
            char confirm[10];
            docDongAnToan(confirm, sizeof(confirm), stdin);
            if (strlen(confirm) > 0 && tolower(confirm[0]) == 'c') {
                if (xoaBenhNhan(dsBenhNhan, soLuongBenhNhan, maXoa_str, tenFileCSV)) {
                    inThongDiepThanhCong("Xoa benh nhan thanh cong!");
                }
                else {
                    inThongDiepLoi("Xoa benh nhan that bai (co the khong tim thay).");
                }
            }
            else {
                printf("Huy xoa benh nhan.\n");
            }
            dungManHinhCho();
            break;
        }
        case 4:
            xoaManHinhConsole();
            inDanhSachBenhNhanConsole(*dsBenhNhan, *soLuongBenhNhan);
            dungManHinhCho();
            break;
        case 5: {
            char tieuChi[50], giaTri[100];
            BenhNhan* ketQua = NULL;
            int soLuongKQ = 0;

            printf("Tim kiem theo (MaDinhDanh, BHYT, SoDienThoai, Ten): ");
            docDongAnToan(tieuChi, sizeof(tieuChi), stdin);
            printf("Nhap gia tri can tim: ");
            docDongAnToan(giaTri, sizeof(giaTri), stdin);

            timKiemBenhNhan(*dsBenhNhan, *soLuongBenhNhan, tieuChi, giaTri, &ketQua, &soLuongKQ);

            xoaManHinhConsole();
            if (soLuongKQ > 0) {
                printf("--- KET QUA TIM KIEM (%d benh nhan) ---\n", soLuongKQ);
                inDanhSachBenhNhanConsole(ketQua, soLuongKQ);
                free(ketQua);
            }
            else {
                printf("Khong tim thay benh nhan nao phu hop.\n");
            }
            dungManHinhCho();
            break;
        }
        case 0:
            printf("Quay lai menu chinh...\n");
            break;
        }
    } while (choice != 0);
}

// Ham menuQuanLyLanKham (da sua de khong nhan tenFileCSV)
// Trong file ui.c

// (Cac include va cac ham khac cua ui.c giu nguyen)
// ...

// Dam bao rang dinh nghia cua ham menuQuanLyLanKham khop voi khai bao trong ui.h:
void menuQuanLyLanKham(
    LanKham** dsLanKham, int* soLuongLanKham,
    BenhNhan* dsBenhNhan, int soLuongBenhNhan,
    BacSi* dsBacSi, int soLuongBacSi,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong,
    const char* fileLanKhamCSV // Day la ten file CSV cho lan kham, vi du "data/lankham.csv"
) {
    int choice;
    do {
        xoaManHinhConsole();
        printf("--- QUAN LY THONG TIN CAC LAN KHAM ---\n");
        printf("1. Xem danh sach tat ca lan kham\n");
        printf("2. Tim kiem lan kham\n");
        printf("3. Sua chi tiet mot lan kham\n"); // Lua chon de sua lan kham
        printf("0. Quay lai menu chinh\n");

        // Dam bao nguoi dung co the chon 3
        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 3);
        if (choice < 0) { choice = 0; } // Xu ly neu nhapSoNguyenCoGioiHan tra ve loi

        switch (choice) {
        case 1:
            xoaManHinhConsole();
            // dsBenhNhan va soLuongBenhNhan duoc truyen vao de hien thi ten benh nhan
            inDanhSachLanKhamConsole(*dsLanKham, *soLuongLanKham, dsBenhNhan, soLuongBenhNhan);
            dungManHinhCho();
            break;
        case 2: {
            char tieuChi[50], giaTri[100];
            LanKham* ketQua = NULL;
            int soLuongKQ = 0;

            printf("Chon tieu chi tim kiem:\n");
            printf("  1. Ma Lan Kham (vd: 1, 2, ...)\n");
            printf("  2. Ma Dinh Danh Benh Nhan (vd: BN001, ...)\n");
            printf("  3. Trang Thai\n");
            printf("  4. Ma Bac Si (vd: BS001, ...)\n");
            int luaChonTieuChi = nhapSoNguyenCoGioiHan("Lua chon tieu chi", 1, 4);

            if (luaChonTieuChi >= 1 && luaChonTieuChi <= 4) {
                switch (luaChonTieuChi) {
                case 1: strcpy(tieuChi, "MaLanKham"); break;
                case 2: strcpy(tieuChi, "MaDinhDanhBN"); break;
                case 3: strcpy(tieuChi, "TrangThai"); break;
                case 4: strcpy(tieuChi, "BacSi"); break;
                }

                if (strcmp(tieuChi, "TrangThai") == 0) {
                    printf("Nhap gia tri Trang Thai can tim. Cac trang thai co the la:\n");
                    printf("  (DangCho, DangKham, DaHoanThanh, DaHuy, DaLo)\n");
                    printf("Gia tri Trang Thai: ");
                }
                else {
                    printf("Nhap gia tri can tim cho '%s': ", tieuChi);
                }
                docDongAnToan(giaTri, sizeof(giaTri), stdin);

                if (strlen(giaTri) > 0) {
                    timKiemLanKham(*dsLanKham, *soLuongLanKham, tieuChi, giaTri, &ketQua, &soLuongKQ);
                    xoaManHinhConsole();
                    if (soLuongKQ > 0) {
                        printf("--- KET QUA TIM KIEM CHO TIEU CHI '%s' = '%s' (%d lan kham) ---\n", tieuChi, giaTri, soLuongKQ);
                        inDanhSachLanKhamConsole(ketQua, soLuongKQ, dsBenhNhan, soLuongBenhNhan);
                        free(ketQua);
                    }
                    else {
                        printf("Khong tim thay lan kham nao phu hop voi tieu chi '%s' = '%s'.\n", tieuChi, giaTri);
                    }
                }
                else {
                    inThongDiepLoi("Gia tri tim kiem khong duoc de trong.");
                }
            }
            else {
                inThongDiepLoi("Lua chon tieu chi khong hop le.");
            }
            dungManHinhCho();
            break;
        }
        case 3: { // Day la case de goi ham sua chi tiet lan kham
            xoaManHinhConsole();
            printf("--- SUA CHI TIET LAN KHAM ---\n");

            // Hien thi danh sach rut gon cac lan kham de nguoi dung de chon (tuy chon)
            // Hoac co the yeu cau nhap ma truc tiep
            // Vi du:
            if (*soLuongLanKham > 0) {
                printf("Danh sach cac lan kham hien co (Ma LK - Ten BN - Trang Thai):\n");
                for (int i = 0; i < *soLuongLanKham; ++i) {
                    char tenBNHienThi[101] = "N/A";
                    // Lay ten benh nhan (tuong tu nhu trong inDanhSachLanKhamConsole)
                    if (strlen((*dsLanKham)[i].tenBenhNhan) > 0 && strcmp((*dsLanKham)[i].tenBenhNhan, "Khong Biet") != 0 && strcmp((*dsLanKham)[i].tenBenhNhan, "Khong Xac Dinh") != 0) {
                        strncpy(tenBNHienThi, (*dsLanKham)[i].tenBenhNhan, 100);
                    }
                    else {
                        for (int j = 0; j < soLuongBenhNhan; ++j) {
                            if (strcmp(dsBenhNhan[j].maDinhDanh, (*dsLanKham)[i].maDinhDanh) == 0) {
                                strncpy(tenBNHienThi, dsBenhNhan[j].ten, 100);
                                break;
                            }
                        }
                    }
                    tenBNHienThi[100] = '\0';
                    printf(" - Ma LK: %d, BN: %s, Ma BN: %s, Trang Thai: %s\n",
                        (*dsLanKham)[i].maLanKham,
                        tenBNHienThi,
                        (*dsLanKham)[i].maDinhDanh,
                        trangThaiLanKhamToString((*dsLanKham)[i].trangThai));
                }
                printf("-------------------------------------------------\n");
            }
            else {
                printf("Chua co lan kham nao trong he thong.\n");
                dungManHinhCho();
                break; // Thoat khoi case 3 neu khong co lan kham nao
            }

            int maLKCanSua = nhapSoNguyenKhongAm("Nhap Ma Lan Kham can sua");

            if (maLKCanSua >= 0) {
                // Goi ham suaChiTietLanKham tu examination.c
                // dsLanKham la LanKham**, nen truyen *dsLanKham (mang cac lan kham)
                // soLuongLanKham la int*, nen truyen *soLuongLanKham (so nguyen)
                // dsBacSi, dsKhoaPhong da la con tro toi mang (BacSi*, KhoaPhong*)
                // fileLanKhamCSV duoc truyen truc tiep
                int ketQuaSua = suaChiTietLanKham(
                    *dsLanKham,
                    *soLuongLanKham,
                    maLKCanSua,
                    dsBacSi,
                    soLuongBacSi,
                    dsKhoaPhong,
                    soLuongKhoaPhong,
                    fileLanKhamCSV
                );
                // Ham suaChiTietLanKham da tu in thong bao thanh cong/loi va luu file
                // Ban co the them xu ly dua tren ketQuaSua neu can (1 la thanh cong, 0 la that bai/huy)
            }
            else if (maLKCanSua == -1) {
                // nhapSoNguyenKhongAm thuong in loi roi, hoac ban co the them:
                // inThongDiepLoi("Ma lan kham nhap vao khong hop le.");
            }
            // Ham suaChiTietLanKham da co dungManHinhCho() ben trong cac nhanh return hoac cuoi vong lap,
            // nen co the khong can goi dungManHinhCho() o day nua, tuy thuoc vao luong cua suaChiTietLanKham.
            // Tuy nhien, de chac chan, ban co the giu lai hoac xem xet cau truc cua suaChiTietLanKham.
            // Neu suaChiTietLanKham luon goi dungManHinhCho() truoc khi return, thi o day khong can.
            // Gia su suaChiTietLanKham da xu ly viec dung man hinh.
            // Neu khong, them: dungManHinhCho(); 
            break;
        }
        case 0:
            printf("Quay lai menu chinh...\n");
            break;
        default:
            inThongDiepLoi("Lua chon khong hop le.");
            dungManHinhCho();
            break;
        }
    } while (choice != 0);
}

// ... (Phan con lai cua ui.c)

// Ham menuQuanLyKhoaPhong (da bao gom chuc nang sua co chon loc)
void menuQuanLyKhoaPhong(KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, const char* tenFileCSV) {
    int choice;
    do {
        xoaManHinhConsole();
        printf("--- QUAN LY KHOA PHONG ---\n");
        printf("1. Them khoa phong moi\n");
        printf("2. Sua thong tin khoa phong\n");
        printf("3. Xoa khoa phong\n");
        printf("4. Xem danh sach khoa phong\n");
        printf("0. Quay lai menu chinh\n");
        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 4);
        if (choice < 0) { choice = 0; }

        switch (choice) {
        case 1: {
            KhoaPhong kpMoi;
            printf("\n--- THEM KHOA PHONG MOI ---\n");
            if (nhapThongTinKhoaPhongTuBanPhim(&kpMoi, *dsKhoaPhong, *soLuongKhoaPhong)) {
                if (themKhoaPhong(dsKhoaPhong, soLuongKhoaPhong, kpMoi, tenFileCSV)) {
                    inThongDiepThanhCong("Them khoa phong moi thanh cong!");
                }
                else {
                    inThongDiepLoi("Them khoa phong moi that bai.");
                }
            }
            else {
                inThongDiepLoi("Thong tin nhap khong hop le hoac ma khoa phong bi trung.");
            }
            dungManHinhCho();
            break;
        }
        case 2: {
            char maSua_str[MAX_LEN_MA_KHOA_PHONG];
            printf("Nhap Ma Khoa Phong can sua: ");
            if (docDongAnToan(maSua_str, sizeof(maSua_str), stdin) == NULL || strlen(maSua_str) == 0) {
                inThongDiepLoi("Ma khoa phong khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int k = 0; maSua_str[k]; k++) maSua_str[k] = toupper(maSua_str[k]);

            int khoaPhongIndex = -1;
            for (int i = 0; i < *soLuongKhoaPhong; ++i) {
                if (strcmp((*dsKhoaPhong)[i].maKhoaPhong, maSua_str) == 0) {
                    khoaPhongIndex = i;
                    break;
                }
            }

            if (khoaPhongIndex != -1) {
                KhoaPhong kpTamSua = (*dsKhoaPhong)[khoaPhongIndex];
                int suaLuaChonNoiBo;

                do {
                    xoaManHinhConsole();
                    printf("--- SUA THONG TIN KHOA PHONG (Ma KP: %s) ---\n", kpTamSua.maKhoaPhong);
                    printf("Thong tin hien tai:\n");
                    printf("  1. Ten Khoa Phong: %s\n", kpTamSua.tenKhoaPhong);
                    printf("  2. Vi Tri        : %s\n", kpTamSua.viTri);
                    printf("  3. Mo Ta         : %s\n", kpTamSua.moTa);
                    printf("-------------------------------------------------\n");
                    printf("Chon thong tin muon sua:\n");
                    printf("  1. Sua Ten Khoa Phong\n");
                    printf("  2. Sua Vi Tri\n");
                    printf("  3. Sua Mo Ta\n");
                    printf("-------------------------------------------------\n");
                    printf("  0. HOAN TAT SUA DOI VA LUU\n");
                    printf("  9. Huy bo tat ca thay doi va quay lai\n");

                    suaLuaChonNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
                    if (suaLuaChonNoiBo < 0) { suaLuaChonNoiBo = 9; }

                    switch (suaLuaChonNoiBo) {
                    case 1: printf("Nhap Ten Khoa Phong moi (hien tai: %s): ", kpTamSua.tenKhoaPhong); docDongAnToan(kpTamSua.tenKhoaPhong, sizeof(kpTamSua.tenKhoaPhong), stdin); break;
                    case 2: printf("Nhap Vi Tri moi (hien tai: %s): ", kpTamSua.viTri); docDongAnToan(kpTamSua.viTri, sizeof(kpTamSua.viTri), stdin); break;
                    case 3: printf("Nhap Mo Ta moi (hien tai: %s): ", kpTamSua.moTa); docDongAnToan(kpTamSua.moTa, sizeof(kpTamSua.moTa), stdin); break;
                    case 0:
                        if (suaThongTinKhoaPhong(*dsKhoaPhong, *soLuongKhoaPhong, maSua_str, kpTamSua, tenFileCSV)) {
                            inThongDiepThanhCong("Da luu cac thay doi cho khoa phong!");
                        }
                        else {
                            inThongDiepLoi("Loi khi luu thay doi cho khoa phong.");
                        }
                        break;
                    case 4: inThongDiepLoi("Da huy bo sua doi."); suaLuaChonNoiBo = 0; break;
                    default: inThongDiepLoi("Lua chon khong hop le."); dungManHinhCho(); break;
                    }
                    if (suaLuaChonNoiBo >= 1 && suaLuaChonNoiBo <= 3) {
                        inThongDiepThanhCong("Da cap nhat truong.");
                        dungManHinhCho();
                    }
                } while (suaLuaChonNoiBo != 0);
            }
            else {
                inThongDiepLoi("Khong tim thay khoa phong voi ma da nhap.");
            }
            dungManHinhCho();
            break;
        }
        case 3: {
            char maXoa[MAX_LEN_MA_KHOA_PHONG];
            printf("Nhap Ma Khoa Phong can xoa: ");
            if (docDongAnToan(maXoa, sizeof(maXoa), stdin) == NULL || strlen(maXoa) == 0) {
                inThongDiepLoi("Ma khoa phong khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int i = 0; maXoa[i]; i++) maXoa[i] = toupper(maXoa[i]);

            printf("Ban co chac chan muon xoa Khoa Phong '%s'? (c/k): ", maXoa);
            char confirm[10];
            docDongAnToan(confirm, sizeof(confirm), stdin);
            if (strlen(confirm) > 0 && tolower(confirm[0]) == 'c') {
                if (xoaKhoaPhong(dsKhoaPhong, soLuongKhoaPhong, maXoa, tenFileCSV)) {
                    inThongDiepThanhCong("Xoa khoa phong thanh cong!");
                }
                else {
                    inThongDiepLoi("Xoa khoa phong that bai (co the khong tim thay).");
                }
            }
            else {
                printf("Huy xoa khoa phong.\n");
            }
            dungManHinhCho();
            break;
        }
        case 4:
            xoaManHinhConsole();
            inDanhSachKhoaPhongConsole(*dsKhoaPhong, *soLuongKhoaPhong);
            dungManHinhCho();
            break;
        case 0:
            printf("Quay lai menu chinh...\n");
            break;
        }
    } while (choice != 0);
}

void menuQuanLyBacSi(BacSi** dsBacSi, int* soLuongBacSi, const char* tenFileCSV,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong) {
    int choice;
    do {
        xoaManHinhConsole();
        printf("--- QUAN LY BAC SI ---\n");
        printf("1. Them bac si moi\n");
        printf("2. Sua thong tin bac si\n");
        printf("3. Xoa bac si\n");
        printf("4. Xem danh sach bac si\n");
        printf("0. Quay lai menu chinh\n");
        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 4);
        if (choice < 0) { choice = 0; }

        switch (choice) {
        case 1: {
            BacSi bsMoi;
            printf("\n--- THEM BAC SI MOI ---\n");
            if (soLuongKhoaPhong == 0) {
                inThongDiepLoi("Chua co khoa phong nao. Vui long them khoa phong truoc khi them bac si.");
            }
            else {
                if (nhapThongTinBacSiTuBanPhim(&bsMoi, *dsBacSi, *soLuongBacSi, dsKhoaPhong, soLuongKhoaPhong)) {
                    if (themBacSi(dsBacSi, soLuongBacSi, bsMoi, tenFileCSV)) {
                        inThongDiepThanhCong("Them bac si moi thanh cong!");
                    }
                    else {
                        inThongDiepLoi("Them bac si moi that bai.");
                    }
                }
                else {
                    inThongDiepLoi("Thong tin nhap khong hop le, ma bac si bi trung hoac ma khoa phong khong ton tai.");
                }
            }
            dungManHinhCho();
            break;
        }
        case 2: {
            char maSua_str[MAX_LEN_MA_BAC_SI];
            printf("Nhap Ma Bac Si can sua: ");
            if (docDongAnToan(maSua_str, sizeof(maSua_str), stdin) == NULL || strlen(maSua_str) == 0) {
                inThongDiepLoi("Ma bac si khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int k = 0; maSua_str[k]; k++) maSua_str[k] = toupper(maSua_str[k]);

            int bacSiIndex = -1;
            for (int i = 0; i < *soLuongBacSi; ++i) {
                if (strcmp((*dsBacSi)[i].maBacSi, maSua_str) == 0) {
                    bacSiIndex = i;
                    break;
                }
            }

            if (bacSiIndex != -1) {
                BacSi bsTamSua = (*dsBacSi)[bacSiIndex];
                int suaLuaChonNoiBo;
                char bufferNhapLieu[200];

                do {
                    xoaManHinhConsole();
                    printf("--- SUA THONG TIN BAC SI (Ma BS: %s) ---\n", bsTamSua.maBacSi);
                    printf("Thong tin hien tai:\n");
                    printf("  1. Ten Bac Si    : %s\n", bsTamSua.tenBacSi);
                    printf("  2. Chuyen Khoa   : %s\n", bsTamSua.chuyenKhoa);
                    printf("  3. So Dien Thoai : %s\n", bsTamSua.soDienThoai);
                    printf("  4. Email         : %s\n", bsTamSua.email);
                    printf("  5. Ma Khoa Phong : %s\n", bsTamSua.maKhoaPhong);
                    printf("-------------------------------------------------\n");
                    printf("Chon thong tin muon sua:\n");
                    printf("  1. Sua Ten Bac Si\n  2. Sua Chuyen Khoa\n  3. Sua So Dien Thoai\n  4. Sua Email\n  5. Sua Ma Khoa Phong\n");
                    printf("-------------------------------------------------\n");
                    printf("  0. HOAN TAT SUA DOI VA LUU\n");
                    printf("  9. Huy bo tat ca thay doi va quay lai\n");

                    suaLuaChonNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
                    if (suaLuaChonNoiBo < 0) { suaLuaChonNoiBo = 9; }

                    switch (suaLuaChonNoiBo) {
                    case 1: printf("Nhap Ten Bac Si moi (hien tai: %s): ", bsTamSua.tenBacSi); docDongAnToan(bsTamSua.tenBacSi, sizeof(bsTamSua.tenBacSi), stdin); break;
                    case 2: printf("Nhap Chuyen Khoa moi (hien tai: %s): ", bsTamSua.chuyenKhoa); docDongAnToan(bsTamSua.chuyenKhoa, sizeof(bsTamSua.chuyenKhoa), stdin); break;
                    case 3: printf("Nhap So Dien Thoai moi (hien tai: %s): ", bsTamSua.soDienThoai); docDongAnToan(bsTamSua.soDienThoai, sizeof(bsTamSua.soDienThoai), stdin); break;
                    case 4: printf("Nhap Email moi (hien tai: %s): ", bsTamSua.email); docDongAnToan(bsTamSua.email, sizeof(bsTamSua.email), stdin); break;
                    case 5: {
                        char maKPMoi_str[MAX_LEN_MA_KHOA_PHONG];
                        printf("Nhap Ma Khoa Phong moi (hien tai: %s): ", bsTamSua.maKhoaPhong);
                        if (docDongAnToan(maKPMoi_str, sizeof(maKPMoi_str), stdin) == NULL || strlen(maKPMoi_str) == 0) {
                            inThongDiepLoi("Ma Khoa Phong khong duoc de trong. Thong tin khong thay doi.");
                            break;
                        }
                        for (int k = 0; maKPMoi_str[k]; k++) maKPMoi_str[k] = toupper(maKPMoi_str[k]);

                        if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKPMoi_str) != NULL) {
                            strcpy(bsTamSua.maKhoaPhong, maKPMoi_str);
                        }
                        else {
                            inThongDiepLoi("Ma Khoa Phong moi khong ton tai. Thong tin khong duoc thay doi.");
                        }
                        break;
                    }
                    case 0:
                        if (suaThongTinBacSi(*dsBacSi, *soLuongBacSi, maSua_str, bsTamSua, dsKhoaPhong, soLuongKhoaPhong, tenFileCSV)) {
                            inThongDiepThanhCong("Da luu cac thay doi cho bac si!");
                        }
                        else {
                            inThongDiepLoi("Loi khi luu thay doi cho bac si.");
                        }
                        break;
                    case 9: inThongDiepLoi("Da huy bo sua doi."); suaLuaChonNoiBo = 0; break;
                    default: inThongDiepLoi("Lua chon khong hop le."); dungManHinhCho(); break;
                    }
                    if (suaLuaChonNoiBo >= 1 && suaLuaChonNoiBo <= 5) {
                        inThongDiepThanhCong("Da cap nhat truong.");
                        dungManHinhCho();
                    }
                } while (suaLuaChonNoiBo != 0);
            }
            else {
                inThongDiepLoi("Khong tim thay bac si voi ma da nhap.");
            }
            dungManHinhCho();
            break;
        }
        case 3: {
            char maXoa[MAX_LEN_MA_BAC_SI];
            printf("Nhap Ma Bac Si can xoa: ");
            if (docDongAnToan(maXoa, sizeof(maXoa), stdin) == NULL || strlen(maXoa) == 0) {
                inThongDiepLoi("Ma bac si khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int i = 0; maXoa[i]; i++) maXoa[i] = toupper(maXoa[i]);

            printf("Ban co chac chan muon xoa Bac Si '%s'? (c/k): ", maXoa);
            char confirm[10];
            docDongAnToan(confirm, sizeof(confirm), stdin);
            if (strlen(confirm) > 0 && tolower(confirm[0]) == 'c') {
                if (xoaBacSi(dsBacSi, soLuongBacSi, maXoa, tenFileCSV)) {
                    inThongDiepThanhCong("Xoa bac si thanh cong!");
                }
                else {
                    inThongDiepLoi("Xoa bac si that bai (co the khong tim thay).");
                }
            }
            else {
                printf("Huy xoa bac si.\n");
            }
            dungManHinhCho();
            break;
        }
        case 4:
            xoaManHinhConsole();
            inDanhSachBacSiConsole(*dsBacSi, *soLuongBacSi);
            dungManHinhCho();
            break;
        case 0:
            // printf("Quay lai menu chinh...\n");
            break;
        }
    } while (choice != 0);
}

void menuQuanLyThuoc(Thuoc** dsThuoc, int* soLuongThuoc, const char* tenFileCSV) {
    int choice;
    do {
        xoaManHinhConsole();
        printf("--- QUAN LY THUOC ---\n");
        printf("1. Them thuoc moi\n");
        printf("2. Sua thong tin thuoc\n");
        printf("3. Xoa thuoc\n");
        printf("4. Xem danh sach thuoc\n");
        printf("0. Quay lai menu chinh\n");
        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 4);
        if (choice < 0) { choice = 0; }

        switch (choice) {
        case 1: {
            Thuoc thuocMoi;
            printf("\n--- THEM THUOC MOI ---\n");
            if (nhapThongTinThuocTuBanPhim(&thuocMoi, *dsThuoc, *soLuongThuoc)) {
                if (themThuoc(dsThuoc, soLuongThuoc, thuocMoi, tenFileCSV)) {
                    inThongDiepThanhCong("Them thuoc moi thanh cong!");
                }
                else {
                    inThongDiepLoi("Them thuoc moi that bai.");
                }
            }
            else {
                inThongDiepLoi("Thong tin nhap khong hop le hoac ma thuoc bi trung.");
            }
            dungManHinhCho();
            break;
        }
        case 2: { // Sua thong tin thuoc (phien ban co chon loc)
            char maSua_str[MAX_LEN_MA_THUOC];
            printf("Nhap Ma Thuoc can sua: ");
            if (docDongAnToan(maSua_str, sizeof(maSua_str), stdin) == NULL || strlen(maSua_str) == 0) {
                inThongDiepLoi("Ma thuoc khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int k = 0; maSua_str[k]; k++) maSua_str[k] = toupper(maSua_str[k]);

            int thuocIndex = -1;
            for (int i = 0; i < *soLuongThuoc; ++i) {
                if (strcmp((*dsThuoc)[i].maThuoc, maSua_str) == 0) {
                    thuocIndex = i;
                    break;
                }
            }

            if (thuocIndex != -1) {
                Thuoc thuocTamSua = (*dsThuoc)[thuocIndex];
                int suaLuaChonNoiBo;

                do {
                    xoaManHinhConsole();
                    printf("--- SUA THONG TIN THUOC (Ma Thuoc: %s) ---\n", thuocTamSua.maThuoc);
                    printf("Thong tin hien tai:\n");
                    printf("  1. Ten Thuoc: %s\n", thuocTamSua.tenThuoc);
                    printf("-------------------------------------------------\n");
                    printf("Chon thong tin muon sua:\n");
                    printf("  1. Sua Ten Thuoc\n");
                    printf("-------------------------------------------------\n");
                    printf("  0. HOAN TAT SUA DOI VA LUU\n");
                    printf("  9. Huy bo tat ca thay doi va quay lai\n");

                    suaLuaChonNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
                    if (suaLuaChonNoiBo < 0) { suaLuaChonNoiBo = 9; }

                    switch (suaLuaChonNoiBo) {
                    case 1:
                        printf("Nhap Ten Thuoc moi (hien tai: %s): ", thuocTamSua.tenThuoc);
                        docDongAnToan(thuocTamSua.tenThuoc, sizeof(thuocTamSua.tenThuoc), stdin);
                        break;
                    case 0:
                        if (suaThongTinThuoc(*dsThuoc, *soLuongThuoc, maSua_str, thuocTamSua, tenFileCSV)) {
                            inThongDiepThanhCong("Da luu cac thay doi cho thuoc!");
                        }
                        else {
                            inThongDiepLoi("Loi khi luu thay doi cho thuoc.");
                        }
                        break;
                    case 2:
                        inThongDiepLoi("Da huy bo sua doi.");
                        suaLuaChonNoiBo = 0;
                        break;
                    default:
                        if (suaLuaChonNoiBo != 1) {
                            inThongDiepLoi("Lua chon khong hop le.");
                            dungManHinhCho();
                        }
                        break;
                    }
                    if (suaLuaChonNoiBo == 1) {
                        inThongDiepThanhCong("Da cap nhat truong.");
                        dungManHinhCho();
                    }
                } while (suaLuaChonNoiBo != 0);
            }
            else {
                inThongDiepLoi("Khong tim thay thuoc voi ma da nhap.");
            }
            dungManHinhCho();
            break;
        }
        case 3: {
            char maXoa[MAX_LEN_MA_THUOC];
            printf("Nhap Ma Thuoc can xoa: ");
            if (docDongAnToan(maXoa, sizeof(maXoa), stdin) == NULL || strlen(maXoa) == 0) {
                inThongDiepLoi("Ma thuoc khong duoc de trong.");
                dungManHinhCho();
                break;
            }
            for (int i = 0; maXoa[i]; i++) maXoa[i] = toupper(maXoa[i]);

            printf("Ban co chac chan muon xoa Thuoc '%s'? (c/k): ", maXoa);
            char confirm[10];
            docDongAnToan(confirm, sizeof(confirm), stdin);
            if (strlen(confirm) > 0 && tolower(confirm[0]) == 'c') {
                if (xoaThuoc(dsThuoc, soLuongThuoc, maXoa, tenFileCSV)) {
                    inThongDiepThanhCong("Xoa thuoc thanh cong!");
                }
                else {
                    inThongDiepLoi("Xoa thuoc that bai (co the khong tim thay).");
                }
            }
            else {
                printf("Huy xoa thuoc.\n");
            }
            dungManHinhCho();
            break;
        }
        case 4:
            xoaManHinhConsole();
            inDanhSachThuocConsole(*dsThuoc, *soLuongThuoc);
            dungManHinhCho();
            break;
        case 0:
            // printf("Quay lai menu chinh...\n");
            break;
        }
    } while (choice != 0);
}



// --- Dinh nghia ham menu chinh ---
void hienThiMenuChinh() {
    xoaManHinhConsole();
    printf("=============================================\n");
    printf("   HE THONG QUAN LY KHAM BENH BENH VIEN XYZ  \n");
    printf("=============================================\n");
    printf("1. Quan ly Benh Nhan\n");
    printf("2. Quan ly Thong Tin Cac Lan Kham\n");
    printf("3. Dang Ky Kham Moi\n");
    printf("4. Goi Benh Nhan Tiep Theo Vao Kham\n");
    printf("5. Cap Nhat Thong Tin Lan Kham Da Goi\n");
    printf("6. Quan Ly/Cap Nhat Don Thuoc (Sau Kham)\n");
    printf("7. Quan ly Khoa Phong\n");
    printf("8. Quan ly Bac Si\n");
    printf("9. Quan ly Danh Muc Thuoc\n");
    printf("10. Xem Hang Doi Kham Benh Hien Tai\n");
    printf("0. Thoat Chuong Trinh\n");
    printf("---------------------------------------------\n");
}

void xuLyLuaChonMenuChinh(
    int choice,
    BenhNhan** dsBenhNhan, int* soLuongBenhNhan, const char* fileBN,
    LanKham** dsLanKham, int* soLuongLanKham, const char* fileLK,
    KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, const char* fileKP,
    BacSi** dsBacSi, int* soLuongBacSi, const char* fileBS,
    Thuoc** dsThuoc, int* soLuongThuoc, const char* fileT,
    DonThuocChiTiet** dsDonThuocChiTiet, int* soLuongDTCT, const char* fileDTCT,
    HangDoiUuTien* hangDoiKhamBenh
) {

    switch (choice) {
    case 1: menuQuanLyBenhNhan(dsBenhNhan, soLuongBenhNhan, fileBN); break;
    case 2: menuQuanLyLanKham(
        dsLanKham, soLuongLanKham,           
        *dsBenhNhan, *soLuongBenhNhan,       
        *dsBacSi, *soLuongBacSi,             
        *dsKhoaPhong, *soLuongKhoaPhong,     
        fileLK                               
    );
        break;
    case 3: {
        xoaManHinhConsole();
        printf("--- DANG KY KHAM MOI ---\n");
        int maLKMoi = dangKyKhamMoi(dsLanKham, soLuongLanKham, dsBenhNhan, soLuongBenhNhan, fileLK, fileBN);
        if (maLKMoi != -1) {
            LanKham* lkMoiPtr = NULL;
            for (int i = 0; i < *soLuongLanKham; ++i) {
                if ((*dsLanKham)[i].maLanKham == maLKMoi && (*dsLanKham)[i].trangThai == LK_DANG_CHO) {
                    lkMoiPtr = &(*dsLanKham)[i];
                    break;
                }
            }
            if (lkMoiPtr) {
                if (themVaoHangDoi(hangDoiKhamBenh, *lkMoiPtr)) {
                    inThongDiepThanhCong("Da them benh nhan vao hang doi cho.");
                }
                else {
                    inThongDiepLoi("Khong them duoc vao hang doi.");
                }
            }
        }
        dungManHinhCho();
        break;
    }
    case 4: {
        xoaManHinhConsole();
        printf("--- GOI BENH NHAN TIEP THEO ---\n");
        if (laHangDoiRong(hangDoiKhamBenh)) {
            printf("Hang doi rong, khong co benh nhan nao de goi.\n");
        }
        else {
            time_t thoiGianGoiHienTai = time(NULL);
            LanKham lkDuocGoi;
            if (layBenhNhanTiepTheo(hangDoiKhamBenh, &lkDuocGoi)) {
                char tenBN[101] = "N/A";
                for (int j = 0; j < *soLuongBenhNhan; ++j) {
                    if (strcmp((*dsBenhNhan)[j].maDinhDanh, lkDuocGoi.maDinhDanh) == 0) { // SUA: So sanh chuoi
                        strncpy(tenBN, (*dsBenhNhan)[j].ten, 100);
                        tenBN[100] = '\0';
                        break;
                    }
                }

                double thoiGianChoGiay = difftime(thoiGianGoiHienTai, lkDuocGoi.gioDangKyThanhCong);
                int tongGiayCho = (int)thoiGianChoGiay;
                int gioCho = tongGiayCho / 3600;
                int phutCho = (tongGiayCho % 3600) / 60;
                int giayChoConLai = (int)fmod(thoiGianChoGiay, 60.0);

                printf("Moi benh nhan tiep theo vao kham:\n");
                printf("  Ma Lan Kham: %d\n", lkDuocGoi.maLanKham);
                printf("  Ma Benh Nhan: %s\n", lkDuocGoi.maDinhDanh); // SUA: %s
                printf("  Ten Benh Nhan: %s\n", tenBN);
                printf("  Muc Do Uu Tien: %s\n", mucDoUuTienToString(lkDuocGoi.mucDoUuTien));

                char gioDangKyStr[30];
                struct tm* tm_info_dangky;
                tm_info_dangky = localtime(&lkDuocGoi.gioDangKyThanhCong);
                if (tm_info_dangky != NULL) {
                    strftime(gioDangKyStr, sizeof(gioDangKyStr), "%H:%M:%S %d/%m/%Y", tm_info_dangky);
                    printf("  Thoi gian dang ky: %s\n", gioDangKyStr);
                }
                else {
                    printf("  Thoi gian dang ky: Khong xac dinh\n");
                }

                if (tongGiayCho < 0) {
                    printf("  Thoi gian cho doi: Thoi gian dang ky khong hop le.\n");
                }
                else if (gioCho > 0) {
                    printf("  Thoi gian cho doi: %d gio %d phut %d giay.\n", gioCho, phutCho, giayChoConLai);
                }
                else if (phutCho > 0) {
                    printf("  Thoi gian cho doi: %d phut %d giay.\n", phutCho, giayChoConLai);
                }
                else {
                    printf("  Thoi gian cho doi: %d giay.\n", giayChoConLai);
                }

                int timThayLKTrongDSChinh = 0;
                for (int i = 0; i < *soLuongLanKham; ++i) {
                    if ((*dsLanKham)[i].maLanKham == lkDuocGoi.maLanKham) {
                        (*dsLanKham)[i].trangThai = LK_DANG_KHAM;
                        (*dsLanKham)[i].ngayGioKham = thoiGianGoiHienTai;

                        ghiDanhSachLanKhamRaFile(fileLK, *dsLanKham, *soLuongLanKham);
                        timThayLKTrongDSChinh = 1;
                        printf("  Trang thai da cap nhat thanh: Dang Kham.\n");

                        char thoiGianGoiKhamStr[30];
                        struct tm* tm_info_goikham;
                        tm_info_goikham = localtime(&(*dsLanKham)[i].ngayGioKham);
                        if (tm_info_goikham != NULL) {
                            strftime(thoiGianGoiKhamStr, sizeof(thoiGianGoiKhamStr), "%H:%M:%S %d/%m/%Y", tm_info_goikham);
                            printf("  Thoi gian goi kham (ghi nhan): %s.\n", thoiGianGoiKhamStr);
                        }
                        else {
                            printf("  Thoi gian goi kham (ghi nhan): Khong xac dinh.\n");
                        }
                        break;
                    }
                }
                if (!timThayLKTrongDSChinh) { inThongDiepLoi("Khong tim thay lan kham trong ds chinh."); }
            }
            else { inThongDiepLoi("Khong the lay benh nhan tu hang doi."); }
        }
        dungManHinhCho();
        break;
    }
    case 5: { // Cap nhat thong tin lan kham da goi
        xoaManHinhConsole();
        printf("--- CAP NHAT THONG TIN LAN KHAM DA GOI ---\n");

        printf("Cac lan kham co the cap nhat (Dang Cho hoac Dang Kham):\n");
        printf("---------------------------------------------------------------------------------------\n");
        printf("| %-7s | %-18s | %-25s | %-12s |\n", "Ma LK", "Ma BN", "Ten Benh Nhan", "Trang Thai");
        printf("---------------------------------------------------------------------------------------\n");
        int countLKPhuHop = 0;
        for (int i = 0; i < *soLuongLanKham; ++i) {
            if ((*dsLanKham)[i].trangThai == LK_DANG_CHO || (*dsLanKham)[i].trangThai == LK_DANG_KHAM) {
                // Su dung tenBenhNhan tu LanKham (neu da them vao struct LanKham)
                // Hoac tra cuu tu dsBenhNhan neu LanKham chi co maDinhDanh
                char tenBNHienThi[101] = "N/A";
                // Gia su LanKham da co truong tenBenhNhan
                if (strlen((*dsLanKham)[i].tenBenhNhan) > 0) {
                    strncpy(tenBNHienThi, (*dsLanKham)[i].tenBenhNhan, 100);
                }
                else { // Neu LanKham chua co ten, hoac ten rong, thu tra cuu
                    const BenhNhan* bn = timBenhNhanTheoMaChuoi(*dsBenhNhan, *soLuongBenhNhan, (*dsLanKham)[i].maDinhDanh);
                    if (bn) {
                        strncpy(tenBNHienThi, bn->ten, 100);
                    }
                }
                tenBNHienThi[100] = '\0';

                char tenBNRutGon[26]; // Cho 25 ky tu hien thi
                rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, 25);

                printf("| %-7d | %-18s | %-25s | %-12s |\n",
                    (*dsLanKham)[i].maLanKham,
                    (*dsLanKham)[i].maDinhDanh,
                    tenBNRutGon,
                    trangThaiLanKhamToString((*dsLanKham)[i].trangThai));
                countLKPhuHop++;
            }
        }
        if (countLKPhuHop == 0) {
            printf("Khong co lan kham nao o trang thai 'Dang Cho' hoac 'Dang Kham' de cap nhat.\n");
        }
        printf("---------------------------------------------------------------------------------------\n");

        if (countLKPhuHop > 0) {
            int maLKCapNhat = nhapSoNguyenKhongAm("Nhap Ma Lan Kham can cap nhat");
            if (maLKCapNhat >= 0) {
                capNhatThongTinLanKham(*dsLanKham, *soLuongLanKham, maLKCapNhat,
                    *dsBacSi, *soLuongBacSi,
                    *dsKhoaPhong, *soLuongKhoaPhong,
                    fileLK);
            }
            else if (maLKCapNhat == -1) {
                // Thong bao loi da duoc in trong nhapSoNguyenKhongAm
            }
        }
        dungManHinhCho();
        break;
    }
    case 6: { // Quan ly / Cap nhat don thuoc
        xoaManHinhConsole();
        printf("--- QUAN LY / CAP NHAT DON THUOC ---\n");

        printf("Cac lan kham da hoan thanh va co chi dinh don thuoc:\n");
        printf("---------------------------------------------------------------------------------------\n");
        printf("| %-7s | %-18s | %-25s | %-10s |\n", "Ma LK/DT", "Ma BN", "Ten Benh Nhan", "Co Don?");
        printf("---------------------------------------------------------------------------------------\n");
        int countLKCoDon = 0;
        for (int i = 0; i < *soLuongLanKham; ++i) {
            if ((*dsLanKham)[i].trangThai == LK_DA_HOAN_THANH && (*dsLanKham)[i].coDonThuoc == 1) {
                char tenBNHienThi[101] = "N/A";
                if (strlen((*dsLanKham)[i].tenBenhNhan) > 0) {
                    strncpy(tenBNHienThi, (*dsLanKham)[i].tenBenhNhan, 100);
                }
                else {
                    const BenhNhan* bn = timBenhNhanTheoMaChuoi(*dsBenhNhan, *soLuongBenhNhan, (*dsLanKham)[i].maDinhDanh);
                    if (bn) {
                        strncpy(tenBNHienThi, bn->ten, 100);
                    }
                }
                tenBNHienThi[100] = '\0';

                char tenBNRutGon[26];
                rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, 25);

                printf("| %-7d | %-18s | %-25s | %-10s |\n",
                    (*dsLanKham)[i].maLanKham, // Ma Lan Kham cung la Ma Don Thuoc
                    (*dsLanKham)[i].maDinhDanh,
                    tenBNRutGon,
                    "Co");
                countLKCoDon++;
            }
        }
        if (countLKCoDon == 0) {
            printf("Khong co lan kham nao da hoan thanh va co don thuoc de cap nhat.\n");
        }
        printf("---------------------------------------------------------------------------------------\n");

        if (countLKCoDon > 0) {
            int maDonThuoc = nhapSoNguyenKhongAm("Nhap Ma Don Thuoc (Ma Lan Kham) de cap nhat chi tiet");
            if (maDonThuoc >= 0) {
                int hopLeDeCapNhatDT = 0;
                int lkIndex = -1;
                for (int i = 0; i < *soLuongLanKham; ++i) {
                    if ((*dsLanKham)[i].maLanKham == maDonThuoc) {
                        lkIndex = i;
                        if ((*dsLanKham)[i].coDonThuoc == 1 && (*dsLanKham)[i].trangThai == LK_DA_HOAN_THANH) {
                            hopLeDeCapNhatDT = 1;
                        }
                        else if ((*dsLanKham)[i].coDonThuoc == 0) {
                            printf("Lan kham %d khong duoc chi dinh don thuoc.\n", maDonThuoc);
                        }
                        else if ((*dsLanKham)[i].trangThai != LK_DA_HOAN_THANH) {
                            printf("Lan kham %d chua hoan thanh, khong the cap nhat don thuoc.\n", maDonThuoc);
                        }
                        break;
                    }
                }
                if (lkIndex == -1 && maDonThuoc >= 0) {
                    printf("Khong tim thay Lan Kham voi Ma %d.\n", maDonThuoc);
                }

                if (hopLeDeCapNhatDT) {
                    // Ham quanLyCapNhatChiTietDonThuoc se xu ly viec them/sua/xoa cac dong thuoc
                    quanLyCapNhatChiTietDonThuoc(maDonThuoc, dsDonThuocChiTiet, soLuongDTCT,
                        *dsThuoc, *soLuongThuoc, fileDTCT);
                }
            }
            else if (maDonThuoc == -1) {
                // Thong bao loi da in
            }
        }
        dungManHinhCho();
        break;
    }
    case 7: menuQuanLyKhoaPhong(dsKhoaPhong, soLuongKhoaPhong, fileKP); break;
    case 8: menuQuanLyBacSi(dsBacSi, soLuongBacSi, fileBS, *dsKhoaPhong, *soLuongKhoaPhong); break;
    case 9: menuQuanLyThuoc(dsThuoc, soLuongThuoc, fileT); break;
    case 10: xoaManHinhConsole(); inHangDoiHienTaiConsole(hangDoiKhamBenh, *dsBenhNhan, *soLuongBenhNhan); dungManHinhCho(); break;
    case 0: break;
    default: inThongDiepLoi("Lua chon khong hop le."); dungManHinhCho(); break;
    }
}
