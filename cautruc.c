#ifndef QLPK_STRUCTS_FULL_H
#define QLPK_STRUCTS_FULL_H

#include <time.h> // Can cho kieu time_t

// Enum Trang Thai Lan Kham
typedef enum {
    LK_DANG_CHO,         // Mới đăng ký, đang trong hàng đợi chờ gọi
    LK_DANG_KHAM,        // Đã được gọi vào, đang được bác sĩ khám
    LK_DA_HOAN_THANH,    // Đã khám xong, cập nhật xong thông tin
    LK_DA_HUY,           // Bệnh nhân hoặc hệ thống hủy lượt khám/lịch hẹn
    LK_DA_LO             // Lượt đăng ký của ngày trước nhưng không khám (missed)
} TrangThaiLanKham;

// --- Cau truc Date ---
typedef struct {
    int ngay;
    int thang;
    int nam;
} Date;

// --- Enum Muc Do Uu Tien ---
typedef enum {
    CAP_CUU = 0,    // Muc uu tien cao nhat
    KHAN_CAP,
    THONG_THUONG,
    TAI_KHAM,
    UU_TIEN_KHAC,
    SO_MUC_UU_TIEN // De de quan ly so luong muc uu tien
} MucDoUuTien;

// --- Enum Gioi Tinh ---
typedef enum {
    GT_NAM,
    GT_NU,
    GT_KHAC
} GioiTinh;

// --- Enum Trang Thai Lich Hen ---
typedef enum {
    HEN_MOI_DAT,
    HEN_DA_XAC_NHAN,
    HEN_DA_HUY,
    HEN_DA_HOAN_THANH,
    HEN_KHONG_DEN
} TrangThaiLichHen;



// === Cau truc Khoa/Phong Kham ===
typedef struct {
    char maKhoaPhong[20];       // Ma duy nhat cua Khoa/Phong
    char tenKhoaPhong[100];     // Ten Khoa/Phong
    char viTri[100];            // Vi tri (vi du: Tang 2, Khu A)
    char moTa[200];             // Mo ta them ve Khoa/Phong
} KhoaPhong;

// === Cau truc Bac Si ===
typedef struct {
    char maBacSi[20];           // Ma so duy nhat cua bac si
    char tenBacSi[100];         // Ho va ten bac si
    char chuyenKhoa[100];       // Chuyen khoa chinh
    char soDienThoai[15];       // So dien thoai lien he
    char email[100];            // Email
    char maKhoaPhong[20];       // Ma Khoa/Phong bac si lam viec (Lien ket voi KhoaPhong)
    // Co the them: Bang cap, lich lam viec co dinh,...
} BacSi;

// === Cau truc Benh Nhan ===
typedef struct {
    int maDinhDanh;             // Ma so duy nhat
    char bhyt[16];              // Ma Bao hiem y te
    char soDienThoai[15];       // So dien thoai
    char ten[100];              // Ho va Ten
    Date ngaySinh;              // Ngay sinh
    int tuoi;                   // Tuoi (luu tru truc tiep)
    GioiTinh gioiTinh;          // Gioi tinh (them vao cho day du)
    char diaChi[200];           // Dia chi (them vao cho day du)
    char tieuSuBenhLy[1000];    // Tom tat tieu su benh ly
    time_t ngayTaoHoSo;         // Ngay tao ho so trong he thong
} BenhNhan;

// === Cau truc Lan Kham ===
typedef struct {
    int maLanKham;              // Ma so duy nhat cho lan kham nay
    int maDinhDanh;             // Lien ket den BenhNhan
    char maBacSi[20];           // Lien ket den BacSi (KHONG LUU TEN O DAY)
    char maKhoaPhong[20];       // Noi thuc hien kham (Lien ket voi KhoaPhong, tuy chon)
    time_t ngayGioKham;         // Ngay va gio thuc te kham benh
    char lyDoKham[500];         // Ly do den kham
    char chanDoan[500];         // Chan doan cua bac si
    char phuongPhapDieuTri[1000]; // Phuong phap dieu tri, thuoc men...
    char ghiChuBacSi[500];      // Ghi chu them
    Date ngayTaiKham;           // Ngay hen tai kham
    int coLichTaiKham;          // Co bao co lich tai kham khong (1 = co, 0 = khong)
    MucDoUuTien mucDoUuTien;    // Muc do uu tien cua lan kham nay
    time_t gioDangKyThanhCong;  // Thoi diem dang ky thanh cong lan kham nay
    int coDonThuoc;             // Co bao co ke don thuoc khong (1=co, 0=khong)
    TrangThaiLanKham trangThai; // TRUONG MOI: Trang thai hien tai cua lan kham
    int maDonThuocChiTiet; //Don thuoc cho lan kham, neu khong co don thuoc thi maDonThuoc = NULL
} LanKham;

// === Cau truc Lich Hen ===
typedef struct {
    int maLichHen;              // Ma duy nhat cua lich hen
    int maDinhDanh;             // Lien ket den BenhNhan
    char maBacSi[20];           // Lien ket den BacSi (neu hen dich danh)
    char maKhoaPhong[20];       // Lien ket den KhoaPhong (neu hen theo khoa)
    time_t ngayGioHen;          // Thoi gian duoc hen
    char lyDoHen[200];          // Ly do hen (kham moi, tai kham...)
    TrangThaiLichHen trangThai; // Trang thai cua lich hen
    char ghiChu[300];           // Ghi chu them cho lich hen
} LichHen;

// === Cau truc Thuoc ===
typedef struct {
    char maThuoc[30];           // Ma duy nhat cua thuoc
    char tenThuoc[150];         // Ten thuong mai hoac ten goc

} Thuoc;

// === Cau truc Chi Tiet Don Thuoc ===
// Moi dong nay la mot loai thuoc duoc ke trong mot lan kham
typedef struct {
    int maDonThuocChiTiet;      // Ma duy nhat cho dong ke thuoc nay
    int maLanKham;              // Lien ket den Lan Kham ma don thuoc nay duoc ke
    char maThuoc[30];           // Lien ket den Thuoc duoc ke
    int soLuong;                // So luong thuoc duoc ke (theo don vi tinh)
    char lieuDung[100];         // Lieu dung (vi du: Ngay 2 lan, moi lan 1 vien)
    char cachDung[100];         // Cach dung (vi du: Uong sau an no)
} DonThuocChiTiet;




#endif // QLPK_STRUCTS_FULL_H
