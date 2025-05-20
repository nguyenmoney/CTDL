#ifndef PATIENT_H
#define PATIENT_H

#include "structs.h"
#include <stdio.h>

#define FILE_BENH_NHAN "data/benhnhan.csv"

BenhNhan* docDanhSachBenhNhan(const char* tenFile, int* soLuongBenhNhan);
void ghiDanhSachBenhNhanRaFile(const char* tenFile, const BenhNhan dsBenhNhan[], int soLuongBenhNhan);
int themBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, BenhNhan benhNhanMoi, const char* tenFile);

// Sua lai tham so maDinhDanhCanSua va maDinhDanhCanXoa thanh const char*
int suaThongTinBenhNhan(BenhNhan dsBenhNhan[], int soLuongBenhNhan, char* maDinhDanhCanSua, BenhNhan thongTinMoi, const char* tenFile);
int xoaBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, char* maDinhDanhCanXoa, const char* tenFile);

void inDanhSachBenhNhanConsole(const BenhNhan dsBenhNhan[], int soLuongBenhNhan);

// giaTriTimKiem cho MaDinhDanh se la chuoi
void timKiemBenhNhan(const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
                     const char* tieuChi, const char* giaTriTimKiem,
                     BenhNhan** ketQuaTimKiem, int* soLuongKetQua);

// Ham nhapThongTinBenhNhanTuBanPhim se nhap maDinhDanh la chuoi va kiem tra tinh duy nhat
int nhapThongTinBenhNhanTuBanPhim(BenhNhan* bn, const BenhNhan dsBenhNhanHienCo[], int soLuongHienCo);
const BenhNhan* timBenhNhanTheoMaChuoi(const BenhNhan dsBenhNhan[], int soLuong, const char* maDD_str);
const char* gioiTinhToString(GioiTinh gt);
GioiTinh stringToGioiTinh(const char* str);

#endif // PATIENT_H
