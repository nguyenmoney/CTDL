#include "doctor.h"
#include "department.h" // De su dung timKhoaPhongTheoMa
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Cho toupper

// Ham tien ich noi bo (static)
static void removeNewlineDoc(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

static void readStringSafeDoc(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        removeNewlineDoc(buffer);
    } else {
        buffer[0] = '\0';
    }
}

BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r");
    if (f == NULL) {
        *soLuong = 0;
        return NULL; // Tra ve NULL neu file khong ton tai
    }

    BacSi* ds = NULL;
    int capacity = 0;
    *soLuong = 0;
    char line[500]; // Buffer cho mot dong CSV bac si

    // Bo qua dong tieu de (neu co)
    // fgets(line, sizeof(line), f); 

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlineDoc(line);
        if (strlen(line) < 5) continue; // Dong qua ngan

        if (*soLuong >= capacity) {
            capacity = (capacity == 0) ? 5 : capacity * 2;
            BacSi* temp = realloc(ds, capacity * sizeof(BacSi));
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachBacSi)");
                free(ds);
                fclose(f);
                *soLuong = 0;
                return NULL;
            }
            ds = temp;
        }

        BacSi bs;
        // Dinh dang CSV: maBacSi,tenBacSi,chuyenKhoa,soDienThoai,email,maKhoaPhong
        char* token;
        
        token = strtok(line, ","); if (token) strncpy(bs.maBacSi, token, sizeof(bs.maBacSi)-1); else continue;
        bs.maBacSi[sizeof(bs.maBacSi)-1] = '\0';

        token = strtok(NULL, ","); if (token) strncpy(bs.tenBacSi, token, sizeof(bs.tenBacSi)-1); else strcpy(bs.tenBacSi, "");
        bs.tenBacSi[sizeof(bs.tenBacSi)-1] = '\0';
        
        token = strtok(NULL, ","); if (token) strncpy(bs.chuyenKhoa, token, sizeof(bs.chuyenKhoa)-1); else strcpy(bs.chuyenKhoa, "");
        bs.chuyenKhoa[sizeof(bs.chuyenKhoa)-1] = '\0';

        token = strtok(NULL, ","); if (token) strncpy(bs.soDienThoai, token, sizeof(bs.soDienThoai)-1); else strcpy(bs.soDienThoai, "");
        bs.soDienThoai[sizeof(bs.soDienThoai)-1] = '\0';

        token = strtok(NULL, ","); if (token) strncpy(bs.email, token, sizeof(bs.email)-1); else strcpy(bs.email, "");
        bs.email[sizeof(bs.email)-1] = '\0';

        token = strtok(NULL, "\n"); // Doc den het dong cho maKhoaPhong
        if (token) strncpy(bs.maKhoaPhong, token, sizeof(bs.maKhoaPhong)-1); else strcpy(bs.maKhoaPhong, "");
        bs.maKhoaPhong[sizeof(bs.maKhoaPhong)-1] = '\0';
        
        ds[*soLuong] = bs;
        (*soLuong)++;
    }

    fclose(f);
    return ds;
}

void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong) {
    FILE* f = fopen(tenFile, "w");
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachBacSiRaFile)");
        return;
    }
    // fprintf(f, "MaBacSi,TenBacSi,ChuyenKhoa,SoDienThoai,Email,MaKhoaPhong\n");

    for (int i = 0; i < soLuong; ++i) {
        fprintf(f, "%s,%s,%s,%s,%s,%s\n",
                dsBacSi[i].maBacSi,
                dsBacSi[i].tenBacSi,
                dsBacSi[i].chuyenKhoa,
                dsBacSi[i].soDienThoai,
                dsBacSi[i].email,
                dsBacSi[i].maKhoaPhong);
    }
    fclose(f);
}

BacSi* timBacSiTheoMa(const BacSi dsBacSi[], int soLuong, const char* maBacSi) {
    if (maBacSi == NULL) return NULL;
    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBacSi[i].maBacSi, maBacSi) == 0) {
            return (BacSi*)&dsBacSi[i];
        }
    }
    return NULL;
}

int nhapThongTinBacSiTuBanPhim(BacSi* bs,
                               const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
                               const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong) {
    char bufferMaBS[sizeof(bs->maBacSi)];
    char bufferMaKP[sizeof(bs->maKhoaPhong)];

    printf("Nhap Ma Bac Si (VD: BS001, toi da %zu ky tu): ", sizeof(bs->maBacSi)-1);
    readStringSafeDoc(bufferMaBS, sizeof(bufferMaBS));
    for(int i = 0; bufferMaBS[i]; i++){ // Chuyen sang chu hoa
        bufferMaBS[i] = toupper(bufferMaBS[i]);
    }

    if (strlen(bufferMaBS) == 0) {
        printf("Ma Bac Si khong duoc de trong.\n");
        return 0;
    }
    if (timBacSiTheoMa(dsBacSiHienCo, soLuongBSHienCo, bufferMaBS) != NULL) {
        printf("Loi: Ma Bac Si '%s' da ton tai!\n", bufferMaBS);
        return 0;
    }
    strncpy(bs->maBacSi, bufferMaBS, sizeof(bs->maBacSi)-1);
    bs->maBacSi[sizeof(bs->maBacSi)-1] = '\0';

    printf("Nhap Ten Bac Si: "); readStringSafeDoc(bs->tenBacSi, sizeof(bs->tenBacSi));
    printf("Nhap Chuyen Khoa: "); readStringSafeDoc(bs->chuyenKhoa, sizeof(bs->chuyenKhoa));
    printf("Nhap So Dien Thoai: "); readStringSafeDoc(bs->soDienThoai, sizeof(bs->soDienThoai));
    printf("Nhap Email: "); readStringSafeDoc(bs->email, sizeof(bs->email));
    
    printf("Nhap Ma Khoa Phong (phai ton tai trong danh sach Khoa Phong): ");
    readStringSafeDoc(bufferMaKP, sizeof(bufferMaKP));
    for(int i = 0; bufferMaKP[i]; i++){ // Chuyen sang chu hoa de so sanh voi ds Khoa Phong
        bufferMaKP[i] = toupper(bufferMaKP[i]);
    }

    if (strlen(bufferMaKP) > 0) { // Neu nguoi dung nhap ma khoa phong
        if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferMaKP) == NULL) {
            printf("Loi: Ma Khoa Phong '%s' khong ton tai! Vui long tao khoa phong truoc hoac nhap lai.\n", bufferMaKP);
            // Co the de trong ma khoa phong neu quy dinh cho phep
            // strcpy(bs->maKhoaPhong, "CHUA_XAC_DINH"); // Hoac return 0 de bat nhap lai
            return 0; // Bat buoc ma khoa phong phai hop le
        }
        strncpy(bs->maKhoaPhong, bufferMaKP, sizeof(bs->maKhoaPhong)-1);
        bs->maKhoaPhong[sizeof(bs->maKhoaPhong)-1] = '\0';
    } else { // Neu nguoi dung de trong ma khoa phong
        printf("Ma Khoa Phong khong duoc de trong.\n"); 
        // strcpy(bs->maKhoaPhong, "CHUA_XAC_DINH"); // Hoac return 0 de bat nhap lai
        return 0; // Bat buoc phai co ma khoa phong
    }

    return 1; // Nhap thanh cong
}


int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro, BacSi bacSiMoi, const char* tenFile) {
    // Kiem tra trung ma da duoc thuc hien trong nhapThongTinBacSiTuBanPhim
    BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro + 1) * sizeof(BacSi));
    if (temp == NULL) {
        perror("Loi cap phat bo nho (realloc themBacSi)");
        return 0;
    }
    *dsBacSiConTro = temp;

    (*dsBacSiConTro)[*soLuongConTro] = bacSiMoi;
    (*soLuongConTro)++;

    ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro);
    return 1;
}

int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
                     const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
                     const char* tenFile) {
    int timThay = 0;
    char maKPMoiUpper[sizeof(thongTinMoi.maKhoaPhong)];

    // Kiem tra ma khoa phong moi (neu co thay doi)
    if (strlen(thongTinMoi.maKhoaPhong) > 0) {
        strncpy(maKPMoiUpper, thongTinMoi.maKhoaPhong, sizeof(maKPMoiUpper)-1);
        maKPMoiUpper[sizeof(maKPMoiUpper)-1] = '\0';
        for(int i=0; maKPMoiUpper[i]; i++) maKPMoiUpper[i] = toupper(maKPMoiUpper[i]);

        if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKPMoiUpper) == NULL) {
            printf("Loi: Ma Khoa Phong moi '%s' khong ton tai!\n", maKPMoiUpper);
            return 0; // Ma khoa phong moi khong hop le
        }
        // Neu hop le, gan ma da duoc UPPERCASE vao thongTinMoi
        strcpy(thongTinMoi.maKhoaPhong, maKPMoiUpper);
    } else { // Neu ma khoa phong moi de trong
         printf("Loi: Ma Khoa Phong khong duoc de trong khi sua.\n");
         return 0;
    }


    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBacSi[i].maBacSi, maBacSiCanSua) == 0) {
            // Giu lai maBacSi goc, chi cap nhat cac thong tin khac
            strcpy(thongTinMoi.maBacSi, dsBacSi[i].maBacSi); 
            dsBacSi[i] = thongTinMoi;
            timThay = 1;
            break;
        }
    }

    if (timThay) {
        ghiDanhSachBacSiRaFile(tenFile, dsBacSi, soLuong);
        return 1;
    }
    return 0; // Khong tim thay
}

int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro, const char* maBacSiCanXoa, const char* tenFile) {
    int viTriCanXoa = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if (strcmp((*dsBacSiConTro)[i].maBacSi, maBacSiCanXoa) == 0) {
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
            (*dsBacSiConTro)[i] = (*dsBacSiConTro)[i + 1];
        }
        (*soLuongConTro)--;

        if (*soLuongConTro == 0) {
            free(*dsBacSiConTro);
            *dsBacSiConTro = NULL;
        } else {
            BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro) * sizeof(BacSi));
            if (temp == NULL && *soLuongConTro > 0) {
                perror("Loi realloc khi xoa bac si (thu nho mang)");
            } else if (*soLuongConTro > 0) {
                 *dsBacSiConTro = temp;
            }
        }
        ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro);
        return 1;
    }
    return 0; // Khong tim thay
}

void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong) {
    printf("\n--- DANH SACH BAC SI ---\n");
    if (soLuong == 0) {
        printf("Khong co bac si nao trong danh sach.\n");
        return;
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n", "STT", "Ma BS", "Ten Bac Si", "Chuyen Khoa", "So Dien Thoai", "Email", "Ma Khoa Phong");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuong; ++i) {
        printf("| %-5d | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n",
               i + 1,
               dsBacSi[i].maBacSi,
               dsBacSi[i].tenBacSi,
               dsBacSi[i].chuyenKhoa,
               dsBacSi[i].soDienThoai,
               dsBacSi[i].email,
               dsBacSi[i].maKhoaPhong);
    }
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
}