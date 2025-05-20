#ifndef DOCTOR_H
#define DOCTOR_H

#include "structs.h" // De su dung BacSi va KhoaPhong (cho viec valid)
#include <stdio.h>

#define FILE_BAC_SI "data/bacsi.csv"

// --- Khai bao cac ham ---

/**
 * @brief Doc danh sach bac si tu file CSV vao mot mang dong.
 *
 * @param tenFile Ten file CSV.
 * @param soLuong Con tro de luu tru so luong bac si da doc.
 * @return Con tro toi mang BacSi, hoac NULL neu loi. Nguoi goi giai phong bo nho.
 */
BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong);

/**
 * @brief Ghi danh sach bac si (tu mang) vao file CSV (ghi de).
 *
 * @param tenFile Ten file CSV.
 * @param dsBacSi Mang chua danh sach bac si.
 * @param soLuong So luong bac si trong mang.
 */
void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong);

/**
 * @brief Them mot bac si moi vao danh sach (trong bo nho va file CSV).
 *
 * @param dsBacSiConTro Con tro toi con tro mang BacSi (de realloc).
 * @param soLuongConTro Con tro toi bien so luong bac si.
 * @param bacSiMoi Thong tin bac si moi can them.
 * @param tenFile Ten file CSV de cap nhat.
 * @return 1 neu them thanh cong, 0 neu that bai (vi du: trung ma).
 */
int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro, BacSi bacSiMoi, const char* tenFile);

/**
 * @brief Sua thong tin mot bac si trong danh sach.
 *
 * @param dsBacSi Mang bac si.
 * @param soLuong So luong bac si.
 * @param maBacSiCanSua Ma bac si can sua.
 * @param thongTinMoi Thong tin moi de cap nhat.
 * @param dsKhoaPhong Mang KhoaPhong (de kiem tra Ma Khoa Phong hop le).
 * @param soLuongKhoaPhong So luong KhoaPhong.
 * @param tenFile Ten file CSV.
 * @return 1 neu sua thanh cong, 0 neu khong tim thay hoac loi.
 */
int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
                     const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong, /* De kiem tra maKhoaPhong */
                     const char* tenFile);

/**
 * @brief Xoa mot bac si khoi danh sach.
 *
 * @param dsBacSiConTro Con tro toi con tro mang BacSi.
 * @param soLuongConTro Con tro toi bien so luong bac si.
 * @param maBacSiCanXoa Ma bac si can xoa.
 * @param tenFile Ten file CSV.
 * @return 1 neu xoa thanh cong, 0 neu khong tim thay hoac loi.
 */
int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro, const char* maBacSiCanXoa, const char* tenFile);

/**
 * @brief In danh sach bac si ra man hinh console.
 *
 * @param dsBacSi Mang bac si.
 * @param soLuong So luong bac si.
 */
void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong);

/**
 * @brief Tim kiem bac si theo ma.
 *
 * @param dsBacSi Mang bac si.
 * @param soLuong So luong bac si.
 * @param maBacSi Ma bac si can tim.
 * @return Con tro toi BacSi tim thay, hoac NULL neu khong tim thay.
 */
BacSi* timBacSiTheoMa(const BacSi dsBacSi[], int soLuong, const char* maBacSi);

/**
 * @brief Nhap thong tin cho mot bac si tu ban phim.
 * Ma Bac Si phai la duy nhat. Ma Khoa Phong phai ton tai.
 * @param bs Con tro toi struct BacSi de dien thong tin.
 * @param dsBacSiHienCo Mang cac bac si hien co (de kiem tra trung ma bac si).
 * @param soLuongBSHienCo So luong bac si hien co.
 * @param dsKhoaPhong Mang KhoaPhong (de kiem tra Ma Khoa Phong hop le).
 * @param soLuongKhoaPhong So luong KhoaPhong.
 * @return 1 neu nhap thanh cong va ma la duy nhat, ma khoa phong hop le; 0 neu nguoc lai.
 */
int nhapThongTinBacSiTuBanPhim(BacSi* bs,
                               const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
                               const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong);

#endif // DOCTOR_H