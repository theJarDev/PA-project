#include <windows.h> 
#include <fstream>
#include <string>
#include <ctime>
#include "resource.h"
using namespace std;

fstream lectorEscritor;

#pragma region Listas ligadas
struct User {
	int IDUser;
	string username;
	string password;
	User* next;
	User* prev;
}*oUser, * aUser;

struct InfoVendedor {
	int IDUser;
	string nombreUs;
	string alias;
	char foto[50];
	InfoVendedor* next;
	InfoVendedor* prev;
}*oInfo, * aInfo;

struct Productos {
	int IDUser;
	int IDProd;
	string nombre;
	int inventario;
	char foto1[50];
	char foto2[50];
	float precio;
	int codigo;
	string marca;
	string desc;
	Productos* next;
	Productos* prev;
}*oProd, * aProd;

struct Envios {
	int IDUser;
	int IDProd;
	int IDEnvio;
	int cantidad;
	string calle;
	string colonia;
	string ciudad;
	string estado;
	string mensaje;
	string fecha;
	Envios* next;
	Envios* prev;
}*oEnvio, * aEnvio;
#pragma endregion

#pragma region Globales
int loggedUser;
int GLOBAL_USER_ID = 1;
int GLOBAL_PRODUCTO_ID = 1;
int GLOBAL_ENVIO_ID = 1;
bool exitWindow = false;
HINSTANCE hgInst;

#define MI_TIMER_GENERADO 125
time_t actualTime;
tm* timeInfo;
#pragma endregion

#pragma region Handlers
HWND hTxtLoginPassword;
HWND hTxtLoginUsername;

HWND hTxtNewPass;
HWND hTxtNewUser;

HWND hTxtNombreVendedor;
HWND hTxtAliasEmpresa;
HWND hTxtFotoVendedor;

HWND hLB_Envios;

HWND hTxtNombreProd;
HWND hTxtCantidadProd;
HWND hTxtPrecioProd;
HWND hTxtCodigoProd;
HWND hTxtMarcaProd;
HWND hTxtDescProd;
HWND hTxtFotoProd1;
HWND hTxtFotoProd2;

HWND hTxtMCantidadProd;
HWND hTxtMPrecioProd;
HWND hTxtMCodigoProd;
HWND hTxtMMarcaProd;
HWND hTxtMDescProd;
HWND hTxtMFotoProd1;
HWND hTxtMFotoProd2;

HWND hCB_EnvioProd;
HWND hTxtCantidadEnvio;
HWND hTxtPrecioEnvio;
HWND hTxtCalleEnvio;
HWND hTxtColoniaEnvio;
HWND hTxtCiudadEnvio;
HWND hTxtEstadoEnvio;
HWND hTxtMensajeEnvio;
HWND hDTCFechaEnvio;

HWND hTxtMEnvioProd;
HWND hTxtMCantidadEnvio;
HWND hTxtMPrecioEnvio;
HWND hTxtMCalleEnvio;
HWND hTxtMColoniaEnvio;
HWND hTxtMCiudadEnvio;
HWND hTxtMEstadoEnvio;
HWND hTxtMMensajeEnvio;
HWND hTxtMFechaEnvio;

HWND LB_BorrarProd;
HWND LB_BorrarEnvio;

HWND hCB_BuscarProd;
HWND hCB_MBuscarProd;
HWND hCB_BuscarEnvio;
HWND hCB_MBuscarEnvio;

HWND hTxtNombre;

HMENU menu3;
#pragma endregion

#pragma region Declaracion de funciones
BOOL CALLBACK fLogin(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fRegister(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fInfoVendedor2(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fAltaEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fMostrarEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fCambioEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fBorrarEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fAltaProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fMostrarProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fCambioProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK fBorrarProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void saveUser(User*);
void loadUser();
void saveInfo(InfoVendedor*);
void loadInfo();
void saveProd(Productos*);
void loadProd();
void saveEnvio(Envios*);
void loadEnvio();
void saveGlobalId();
void loadGlobalId();
void freeMemory();
#pragma endregion

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	loadGlobalId();
	oUser = aUser = NULL;
	hgInst = hInst;
	HWND hLogin = CreateDialog(hInst, MAKEINTRESOURCE(DLG_INICIARSESION), NULL, fLogin);

	ShowWindow(hLogin, SW_SHOW);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

#pragma region Callbacks
BOOL CALLBACK fLogin(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadUser();
		hTxtLoginUsername = GetDlgItem(hwnd, EB_USUARIO);
		hTxtLoginPassword = GetDlgItem(hwnd, EB_CONTRASENA);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_SALIRDELPROGRAMA_SALIR: {
			exitWindow = true;
			DestroyWindow(hwnd);
		}break;

		case BTN_NUEVO: {
			HWND hRegister = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_REGISTROUSUARIO), NULL, fRegister);
			ShowWindow(hRegister, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}break;

		case BTN_ACEPTARUSUARIO: {
			char txtUser[50];
			char txtPass[50];

			int txtLengthUser = GetWindowTextLength(hTxtLoginUsername);
			int txtLengthPass = GetWindowTextLength(hTxtLoginPassword);

			if (txtLengthUser < 1 || txtLengthPass < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO LOGIN", MB_ICONERROR);
				break;
			}

			GetWindowText(hTxtLoginUsername, txtUser, ++txtLengthUser);
			string sUser(txtUser);
			GetWindowText(hTxtLoginPassword, txtPass, ++txtLengthPass);
			string sPass(txtPass);

			if (oUser != NULL) {
				bool found = true;
				while (aUser != NULL) {
					if (aUser->username.compare(sUser) == 0 && aUser->password.compare(sPass) == 0)
						break;

					if (aUser->next == NULL) {
						found = false;
						break;
					}
					aUser = aUser->next;
				}
				if (found) {
					loggedUser = aUser->IDUser;
					aUser = oUser;
					freeMemory();
					HWND hPrincipal = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_PRINCIPAL), NULL, fPrincipal);
					ShowWindow(hPrincipal, SW_SHOW);
					exitWindow = false;
					DestroyWindow(hwnd);
					break;
				}
				else {
					aUser = oUser;
					MessageBox(hwnd, "Ningun usuario coincide con tus crendenciales", "NO LOGIN", MB_ICONERROR);
					break;
				}
			}
			else {
				MessageBox(hwnd, "No hay Usuarios registrados", "NO LOGIN", MB_ICONERROR);
				break;
			}

		}break;
		}
	}break;

	case WM_CLOSE:
		break;

	case WM_DESTROY: {
		if (exitWindow) {
			freeMemory();
			DestroyWindow(hwnd);
			PostQuitMessage(117);
		}
	}break;

	}return FALSE;
}

BOOL CALLBACK fRegister(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		hTxtNewPass = GetDlgItem(hwnd, EB_NU_CONTRASENA);
		hTxtNewUser = GetDlgItem(hwnd, EB_NU_USUARIO);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_SALIRDELPROGRAMA_SALIR: {
			exitWindow = true;
			DestroyWindow(hwnd);
		}break;

		case BTN_NU_CREAR: {
			bool validar = true;
			char txtUser[50];
			char txtPass[50];

			int txtLengthUser = GetWindowTextLength(hTxtNewUser);
			int txtLengthPass = GetWindowTextLength(hTxtNewPass);

			if (txtLengthUser < 1 || txtLengthPass < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO REGISTRADO", MB_ICONERROR);
				break;
			}

			GetWindowText(hTxtNewUser, txtUser, ++txtLengthUser);
			string sUser(txtUser);
			GetWindowText(hTxtNewPass, txtPass, ++txtLengthPass);
			string sPass(txtPass);

			if (oUser != NULL) {
				bool found = true;
				while (aUser->username.compare(sUser) != 0) {
					if (aUser->next == NULL) {
						found = false;
						break;
					}
					aUser = aUser->next;
				}
				aUser = oUser;
				if (found) {
					validar = false;
					MessageBox(hwnd, "Usuario repetido", "NO REGISTRO", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sUser.length(); i++) {
				bool esNum = sUser[i] >= '0' && sUser[i] <= '9';
				if (esNum) {
					validar = false;
					MessageBox(hwnd, "Usuario\nNo debe contener texto numerico", "NO GUARDADO", MB_ICONERROR);
					break;
				}
			}
			if (validar == true) {
				if (oUser == NULL) {
					oUser = new User;
					oUser->password = sPass;
					oUser->username = sUser;
					oUser->IDUser = GLOBAL_USER_ID++;
					oUser->prev = NULL;
					oUser->next = NULL;
					loggedUser = oUser->IDUser;
					aUser = oUser;
				}
				else {
					while (aUser->next != NULL)
						aUser = aUser->next;
					aUser->next = new User;
					aUser->next->prev = aUser;
					aUser = aUser->next;
					aUser->password = sPass;
					aUser->username = sUser;
					aUser->IDUser = GLOBAL_USER_ID++;
					aUser->next = NULL;
					loggedUser = aUser->IDUser;
					aUser = oUser;
				}
				saveUser(aUser);
				saveGlobalId();
				aUser = oUser;
				freeMemory();
				HWND hInfoVendedor = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_INFORMACION_VENDEDOR), NULL, fInfoVendedor);
				ShowWindow(hInfoVendedor, SW_SHOW);
				exitWindow = false;
				DestroyWindow(hwnd);
			}
		}break;

		case BTN_NU_CANCELAR: {
			HWND hLogin = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_INICIARSESION), NULL, fLogin);
			ShowWindow(hLogin, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}
		}
	}break;

	case WM_CLOSE:
		break;

	case WM_DESTROY: {
		if (exitWindow) {
			freeMemory();
			PostQuitMessage(117);
			DestroyWindow(hwnd);
		}
	}break;

	}return FALSE;
}

BOOL CALLBACK fInfoVendedor(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();
		EnableWindow(GetDlgItem(hwnd, BTN_UCANCELAR), 0);
		hTxtNombreVendedor = GetDlgItem(hwnd, EB_UNOMBRE);
		hTxtAliasEmpresa = GetDlgItem(hwnd, EB_UALIAS);
		hTxtFotoVendedor = GetDlgItem(hwnd, EB_UFOTO);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {

		case ID_SALIRDELPROGRAMA_SALIR:
			MessageBox(hwnd, "Debes llenar la informacion de usuario", "Informacion incompleta", MB_ICONEXCLAMATION);
			break;

		case BTN_UFOTO: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_UFOTO);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoVendedor, cDirFile);
			}
		}break;

		case BTN_UGUARDAR: {
			bool validar = true;
			char txtNombre[50];
			char txtAlias[50];
			char txtFoto[50];

			int txtLengthNombre = GetWindowTextLength(hTxtNombreVendedor);
			int txtLengthAlias = GetWindowTextLength(hTxtAliasEmpresa);
			int txtLengthFoto = GetWindowTextLength(hTxtFotoVendedor);

			if (txtLengthNombre < 1 || txtLengthAlias < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO GUARDADO", MB_ICONERROR);
				break;
			}

			GetWindowText(hTxtNombreVendedor, txtNombre, ++txtLengthNombre);
			string sNombre(txtNombre);

			GetWindowText(hTxtAliasEmpresa, txtAlias, ++txtLengthAlias);
			for (int i = 0; i < txtLengthAlias; i++) {
				txtAlias[i] = toupper(txtAlias[i]);
			}
			string sAlias(txtAlias);

			GetWindowText(hTxtFotoVendedor, txtFoto, ++txtLengthFoto);
			if (txtFoto[0] == NULL) {
				validar = false;
				MessageBox(hwnd, "Debes insertar una foto", "NO GUARDADO", MB_ICONERROR);
			}

			for (int i = 0; i < sNombre.length(); i++) {
				bool esNum = sNombre[i] >= '0' && sNombre[i] <= '9';
				if (esNum) {
					validar = false;
					MessageBox(hwnd, "Nombre\nNo debe contener texto numerico", "NO GUARDADO", MB_ICONERROR);
					break;
				}
			}

			if (validar) {
				if (oInfo == NULL) {
					oInfo = new InfoVendedor;
					oInfo->nombreUs = sNombre;
					oInfo->alias = sAlias;
					strcpy(oInfo->foto, txtFoto);
					oInfo->IDUser = loggedUser;
					oInfo->prev = NULL;
					oInfo->next = NULL;
					aInfo = oInfo;
				}
				else {
					while (aInfo->next != NULL)
						aInfo = aInfo->next;
					aInfo->next = new InfoVendedor;
					aInfo->next->prev = aInfo;
					aInfo = aInfo->next;
					aInfo->nombreUs = sNombre;
					aInfo->alias = sAlias;
					strcpy(aInfo->foto, txtFoto);
					aInfo->IDUser = loggedUser;
					aInfo->next = NULL;
					aInfo = oInfo;
				}
				saveInfo(aInfo);
				aInfo = oInfo;

				freeMemory();
				HWND hPrincipal = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_PRINCIPAL), NULL, fPrincipal);
				ShowWindow(hPrincipal, SW_SHOW);
				exitWindow = false;
				DestroyWindow(hwnd);
			}
		}break;

		}
	}break;

	case WM_CLOSE: {
	}break;

	case WM_DESTROY: {

	}break;

	}return FALSE;
}

BOOL CALLBACK fInfoVendedor2(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();

		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}

		hTxtNombreVendedor = GetDlgItem(hwnd, EB_UNOMBRE);
		hTxtAliasEmpresa = GetDlgItem(hwnd, EB_UALIAS);
		hTxtFotoVendedor = GetDlgItem(hwnd, EB_UFOTO);

		SetWindowText(hTxtNombreVendedor, aInfo->nombreUs.c_str());
		SetWindowText(hTxtAliasEmpresa, aInfo->alias.c_str());
		SetWindowText(hTxtFotoVendedor, aInfo->foto);

		HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_UFOTO);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
		SetWindowText(hTxtFotoVendedor, aInfo->foto);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_SALIRDELPROGRAMA_SALIR: {
			aInfo = oInfo;
			freeMemory();
			EndDialog(hwnd, 0);
		}break;

		case BTN_UFOTO: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_UFOTO);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoVendedor, cDirFile);
			}
		}break;

		case BTN_UGUARDAR: {
			bool validar = true;
			char txtNombre[50];
			char txtAlias[50];
			char txtFoto[50];

			int txtLengthNombre = GetWindowTextLength(hTxtNombreVendedor);
			int txtLengthAlias = GetWindowTextLength(hTxtAliasEmpresa);
			int txtLengthFoto = GetWindowTextLength(hTxtFotoVendedor);

			if (txtLengthNombre < 1 || txtLengthAlias < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO GUARDADO", MB_ICONERROR);
				break;
			}

			GetWindowText(hTxtNombreVendedor, txtNombre, ++txtLengthNombre);
			string sNombre(txtNombre);
			GetWindowText(hTxtAliasEmpresa, txtAlias, ++txtLengthAlias);
			for (int i = 0; i < txtLengthAlias; i++) {
				txtAlias[i] = toupper(txtAlias[i]);
			}
			string sAlias(txtAlias);

			aInfo = oInfo;
			while (aInfo != NULL) {
				if (aInfo->IDUser == loggedUser)
					break;
				aInfo = aInfo->next;
			}

			for (int i = 0; i < sNombre.length(); i++) {
				bool esNum = sNombre[i] >= '0' && sNombre[i] <= '9';
				if (esNum) {
					validar = false;
					MessageBox(hwnd, "Nombre\nNo debe contener texto numerico", "NO GUARDADO", MB_ICONERROR);
					break;
				}
			}

			GetWindowText(hTxtFotoVendedor, txtFoto, ++txtLengthFoto);

			if (validar == true) {
				aInfo->nombreUs = sNombre;
				aInfo->alias = sAlias;
				strcpy(oInfo->foto, txtFoto);
				aInfo = oInfo;

				saveInfo(aInfo);
				aInfo = oInfo;
				freeMemory();
				EndDialog(hwnd, 1);
			}
		}break;

		}
	}break;

	case WM_CLOSE:
		break;

	case WM_DESTROY:
		break;

	}return FALSE;
}

BOOL CALLBACK fPrincipal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		time(&actualTime);
		timeInfo = localtime(&actualTime);

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_M);
		HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_M);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;
		freeMemory();

		loadEnvio();
		hLB_Envios = GetDlgItem(hwnd, LB_PEDIDOS);
		while (aEnvio != NULL) {
			if (aEnvio->IDUser == loggedUser)
				SendMessage(hLB_Envios, LB_ADDSTRING, NULL, (LPARAM)aEnvio->fecha.c_str());
			aEnvio = aEnvio->next;
		}

		aEnvio = oEnvio;

		freeMemory();
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {

		case LB_PEDIDOS: {
			if (HIWORD(wparam) == LBN_SELCHANGE) {

				loadEnvio();
				char cEnvios[50];
				int selIndex = (int)SendMessage(hLB_Envios, LB_GETCURSEL, NULL, NULL);
				SendMessage(hLB_Envios, LB_GETTEXT, (WPARAM)selIndex, (LPARAM)cEnvios);
				string sFechaE(cEnvios);
				bool found = true;

				if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(6, 4)) && (timeInfo->tm_mon + 1) == stoi(sFechaE.substr(3, 2)) && timeInfo->tm_mday < stoi(sFechaE.substr(0, 2)))
					MessageBox(hwnd, "Procesando envio", cEnvios, MB_OK);
				else if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(6, 4)) && (timeInfo->tm_mon + 1) < stoi(sFechaE.substr(3, 2)))
					MessageBox(hwnd, "Procesando envio", cEnvios, MB_OK);
				else if ((timeInfo->tm_year + 1900) < stoi(sFechaE.substr(6, 4)))
					MessageBox(hwnd, "Procesando envio", cEnvios, MB_OK);
				else
					MessageBox(hwnd, "El envio esta ruta de entrega", cEnvios, MB_OK);

				aUser = oUser;
				freeMemory();
			}
		}break;

		case ID_SALIR_SALIR: {
			int msgboxID = MessageBox(hwnd, "¿Desea salir?", "SALIR", MB_ICONEXCLAMATION | MB_YESNO);
			if (msgboxID == IDYES) {
				exitWindow = true;
				DestroyWindow(hwnd);
			}
		}break;

		case ID_ENV40010: {
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
		}break;

		case ID_PRODUCTOS_MOSTRAR: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
		}break;

		case ID_PRODUCTOS_CAMBIARINFORMACI40008: {
			freeMemory();
			int hCambiarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_EDITARPRODUCTO), hwnd, fCambioProd);
		}break;

		case ID_PRODUCTOS_DARDEBAJA: {
			int hBajaProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_BORRARPRODUCTO), hwnd, fBorrarProd);
		}break;

		case ID_PRODUCTOS_AGREGAR: {
			freeMemory();
			int hAltaProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_AGREGARPRODUCTO), hwnd, fAltaProd);
		}break;

		case ID_VER_INFORMACI40001: {
			freeMemory();
			int hInfoVend = DialogBox(hgInst, MAKEINTRESOURCE(DLG_INFORMACION_VENDEDOR), hwnd, fInfoVendedor2);
		}break;

		case BTN_ENUEVO: {
			freeMemory();
			HWND hAltaEnvio = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_AGREGARENVIO), NULL, fAltaEnvio);
			ShowWindow(hAltaEnvio, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}break;

		case BTN_EMOSTRAR: {
			int mostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
		}break;

		case BTN_ECAMBIO: {
			freeMemory();
			HWND hCambioEnvio = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_EDITARENVIO), NULL, fCambioEnvio);
			ShowWindow(hCambioEnvio, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}break;

		case BTN_EBAJA: {
			freeMemory();
			HWND hBajaEnvio = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_BORRARENVIO), NULL, fBorrarEnvio);
			ShowWindow(hBajaEnvio, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}break;
		}break;
	}break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:{
		if (exitWindow) {
			freeMemory();
			PostQuitMessage(117);
		}
	}break;

	}return FALSE;
}

BOOL CALLBACK fAltaProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_AP);
		HBITMAP hFotoPerfil = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagenPerfil = GetDlgItem(hwnd, PC_AP);
		SendMessage(hPbImagenPerfil, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFotoPerfil);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;


		loadProd();
		hTxtNombreProd = GetDlgItem(hwnd, EB_PNOMBRE);
		hTxtCantidadProd = GetDlgItem(hwnd, EB_PCANTIDAD);
		hTxtPrecioProd = GetDlgItem(hwnd, EB_PPRECIO);
		hTxtCodigoProd = GetDlgItem(hwnd, EB_PCODIGO);
		hTxtMarcaProd = GetDlgItem(hwnd, EB_PMARCA);
		hTxtDescProd = GetDlgItem(hwnd, EB_PDESCRIPCION);
		hTxtFotoProd1 = GetDlgItem(hwnd, EB_PFOTO1);
		hTxtFotoProd2 = GetDlgItem(hwnd, EB_PFOTO2);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadProd();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadProd();
		}break;

		case ID_REGRESAR_MEN40018: {
			freeMemory();
			EndDialog(hwnd, 0);
		}break;

		case BTN_PFOTO: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_PFOTO1);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoProd1, cDirFile);
			}
		}break;

		case BTN_PFOTO2: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_PFOTO2);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoProd2, cDirFile);
			}
		}break;

		case BTN_PAGREGAR: {
			bool validar = true;

			char txtNombreProd[50];
			char txtCantidadProd[50];
			char txtPrecioProd[50];
			char txtCodigoProd[50];
			char txtMarcaProd[50];
			char txtDescProd[50];
			char txtFotoProd1[50];
			char txtFotoProd2[50];

			int txtLengthNombre = GetWindowTextLength(hTxtNombreProd);
			int txtLengthCantidad = GetWindowTextLength(hTxtCantidadProd);
			int txtLengthPrecio = GetWindowTextLength(hTxtPrecioProd);
			int txtLengthCodigo = GetWindowTextLength(hTxtCodigoProd);
			int txtLengthMarca = GetWindowTextLength(hTxtMarcaProd);
			int txtLengthDesc = GetWindowTextLength(hTxtDescProd);
			int txtLengthFoto1 = GetWindowTextLength(hTxtFotoProd1);
			int txtLengthFoto2 = GetWindowTextLength(hTxtFotoProd2);

			if (txtLengthNombre < 1 || txtLengthCantidad < 1 || txtLengthPrecio < 1 || txtLengthCodigo < 1 || txtLengthMarca < 1 || txtLengthDesc < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			GetWindowText(hTxtNombreProd, txtNombreProd, ++txtLengthNombre);
			string sNombreP(txtNombreProd);
			GetWindowText(hTxtCantidadProd, txtCantidadProd, ++txtLengthCantidad);
			string sCantidadP(txtCantidadProd);
			GetWindowText(hTxtPrecioProd, txtPrecioProd, ++txtLengthPrecio);
			string sPrecioP(txtPrecioProd);
			GetWindowText(hTxtCodigoProd, txtCodigoProd, ++txtLengthCodigo);
			string sCodigoP(txtCodigoProd);
			GetWindowText(hTxtMarcaProd, txtMarcaProd, ++txtLengthMarca);
			string sMarcaP(txtMarcaProd);
			GetWindowText(hTxtDescProd, txtDescProd, ++txtLengthDesc);
			string sDescP(txtDescProd);
			GetWindowText(hTxtFotoProd1, txtFotoProd1, ++txtLengthFoto1);
			GetWindowText(hTxtFotoProd2, txtFotoProd2, ++txtLengthFoto2);

			if (oProd != NULL) {
				bool found = true;
				while (aProd->nombre.compare(sNombreP) != 0) {
					if (aProd->next == NULL) {
						found = false;
						break;
					}
					aProd = aProd->next;
				}
				aProd = oProd;
				if (found) {
					validar = false;
					MessageBox(hwnd, "Nombre repetido", "NO REGISTRO", MB_ICONERROR);
					break;
				}
			}

			if (txtFotoProd1[0] == NULL || txtFotoProd2[0] == NULL) {
				validar = false;
				MessageBox(hwnd, "Debes insertar dos fotos", "NO GUARDADO", MB_ICONERROR);
			}

			for (int i = 0; i < sCantidadP.length(); i++) {
				bool esNum = sCantidadP[i] >= '0' && sCantidadP[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Cantidad\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sPrecioP.length(); i++) {
				bool esNum = sPrecioP[i] >= '0' && sPrecioP[i] <= '9' || sPrecioP[i] == '.';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Precio\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sCodigoP.length(); i++) {
				bool esNum = sCodigoP[i] >= '0' && sCodigoP[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Código\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
				else {
					if (oProd != NULL) {
						bool found = true;
						while (aProd->codigo != stoi(sCodigoP)) {
							if (aProd->next == NULL) {
								found = false;
								break;
							}
							aProd = aProd->next;
						}
						aProd = oProd;
						if (found) {
							validar = false;
							MessageBox(hwnd, "Código repetido", "NO SE DIO DE ALTA", MB_ICONERROR);
							break;
						}
					}

				}
			}

			

			if (validar) {
				if (oProd == NULL) {
					oProd = new Productos;
					oProd->nombre = sNombreP;
					oProd->inventario = stoi(sCantidadP);
					oProd->precio = stof(sPrecioP);
					oProd->codigo = stoi(sCodigoP);
					oProd->marca = sMarcaP;
					oProd->desc = sDescP;
					strcpy(oProd->foto1, txtFotoProd1);
					strcpy(oProd->foto2, txtFotoProd2);
					oProd->IDUser = loggedUser;
					oProd->IDProd = GLOBAL_PRODUCTO_ID++;
					oProd->prev = NULL;
					oProd->next = NULL;
					aProd = oProd;
				}
				else {
					while (aProd->next != NULL)
						aProd = aProd->next;
					aProd->next = new Productos;
					aProd->next->prev = aProd;
					aProd = aProd->next;
					aProd->nombre = sNombreP;
					aProd->inventario = stoi(sCantidadP);
					aProd->precio = stof(sPrecioP);
					aProd->codigo = stoi(sCodigoP);
					aProd->marca = sMarcaP;
					aProd->desc = sDescP;
					strcpy(aProd->foto1, txtFotoProd1);
					strcpy(aProd->foto2, txtFotoProd2);
					aProd->IDUser = loggedUser;
					aProd->IDProd = GLOBAL_PRODUCTO_ID++;
					aProd->next = NULL;
					aProd = oProd;
				}

				saveGlobalId();
				saveProd(aProd);
				aProd = oProd;
				freeMemory();
				EndDialog(hwnd, 1);
			}
		}break;

		}break;
	}break;

	case WM_CLOSE: {

	}break;

	case WM_DESTROY: {

	}break;

	}return FALSE;
}

BOOL CALLBACK fMostrarProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		menu3 = GetMenu(hwnd);
		EnableMenuItem(menu3, ID_INFORMACI40016, 0x00000001L);
		EnableMenuItem(menu3, ID_INFORMACI40017, 0x00000001L);

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_MP);
		HBITMAP hFotoPerfil = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagenPerfil = GetDlgItem(hwnd, PC_MP);
		SendMessage(hPbImagenPerfil, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFotoPerfil);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;


		loadProd();
		hCB_MBuscarProd = GetDlgItem(hwnd, CB_MPNOMBRE);
		while (aProd != NULL) {
			if (loggedUser == aProd->IDUser) {
				SendMessage(hCB_MBuscarProd, CB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
			}
			aProd = aProd->next;
		}
		aProd = oProd;

		hTxtMCantidadProd = GetDlgItem(hwnd, EB_MPCANTIDAD);
		hTxtMPrecioProd = GetDlgItem(hwnd, EB_MPPRECIO);
		hTxtMCodigoProd = GetDlgItem(hwnd, EB_MPCODIGO);
		hTxtMMarcaProd = GetDlgItem(hwnd, EB_MPMARCA);
		hTxtMDescProd = GetDlgItem(hwnd, EB_MPDESCRIPCION);
		hTxtMFotoProd1 = GetDlgItem(hwnd, EB_MPFOTO1);
		hTxtMFotoProd2 = GetDlgItem(hwnd, EB_MPFOTO2);

		
	}break;

	case WM_COMMAND:{
		switch (LOWORD(wparam)) {

		case CB_MPNOMBRE: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				char cProd[50];

				int selIndex = (int)SendMessage(hCB_MBuscarProd, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_MBuscarProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

				bool found = true;
				while (aProd != NULL) {
					if (aProd->nombre == cProd)
						break;
					if (aProd->next == NULL) {
						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (found) {
					SetWindowText(hTxtMCantidadProd, to_string(aProd->inventario).c_str());
					SetWindowText(hTxtMPrecioProd, to_string(aProd->precio).c_str());
					SetWindowText(hTxtMCodigoProd, to_string(aProd->codigo).c_str());
					SetWindowText(hTxtMMarcaProd, aProd->marca.c_str());
					SetWindowText(hTxtMDescProd, aProd->desc.c_str());
					SetWindowText(hTxtMFotoProd1, aProd->foto1);
					SetWindowText(hTxtMFotoProd2, aProd->foto2);

					HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aProd->foto1, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPbImagen = GetDlgItem(hwnd, PC_MPFOTO1);
					SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
					SetWindowText(hTxtMFotoProd1, aProd->foto1);

					hFoto = (HBITMAP)LoadImage(NULL, aProd->foto2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					hPbImagen = GetDlgItem(hwnd, PC_MPFOTO2);
					SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
					SetWindowText(hTxtMFotoProd2, aProd->foto2);
				}
				aProd = oProd;
			}
		}break;

		case ID_REGRESAR_MEN40018: {
			aProd = oProd;
			freeMemory();
			EndDialog(hwnd, 0);
			break;
		}

		}
	}break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:
		break;

	}return FALSE;
}

BOOL CALLBACK fCambioProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadProd();
		hCB_BuscarProd = GetDlgItem(hwnd, CB_CPBUSCAR);
		while (aProd != NULL) {
			if (loggedUser == aProd->IDUser) {
				SendMessage(hCB_BuscarProd, CB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
			}
			aProd = aProd->next;
		}
		aProd = oProd;

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_CP);
		HBITMAP hFotoPerfil = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagenPerfil = GetDlgItem(hwnd, PC_CP);
		SendMessage(hPbImagenPerfil, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFotoPerfil);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		hTxtNombreProd = GetDlgItem(hwnd, EB_CPNOMBRE);
		hTxtCantidadProd = GetDlgItem(hwnd, EB_CPCANTIDAD);
		hTxtPrecioProd = GetDlgItem(hwnd, EB_CPPRECIO);
		hTxtCodigoProd = GetDlgItem(hwnd, EB_CPCODIGO);
		hTxtMarcaProd = GetDlgItem(hwnd, EB_CPMARCA);
		hTxtDescProd = GetDlgItem(hwnd, EB_CPDESCRIPCION);
		hTxtFotoProd1 = GetDlgItem(hwnd, EB_CPFOTO1);
		hTxtFotoProd2 = GetDlgItem(hwnd, EB_CPFOTO2);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {

		case BTN_CPFOTO: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_CPFOTO1);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoProd1, cDirFile);
			}
		}break;

		case BTN_CPFOTO2: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char cDirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFile = cDirFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "Imagenes bmp\0*.bmp\0Todos los Archivos\0*.*\0";

			if (GetOpenFileName(&ofn)) {
				HBITMAP hFoto = (HBITMAP)LoadImage(NULL, cDirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				HWND hPbImagen = GetDlgItem(hwnd, PC_CPFOTO2);
				SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
				SetWindowText(hTxtFotoProd2, cDirFile);
			}
		}break;

		case CB_CPBUSCAR: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hwnd, BTN_CPFOTO), 1);
				EnableWindow(GetDlgItem(hwnd, BTN_CPFOTO2), 1);
				EnableWindow(GetDlgItem(hwnd, BTN_CPCAMBIO), 1);
				char cProd[50];

				int selIndex = (int)SendMessage(hCB_BuscarProd, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_BuscarProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

				bool found = true;
				while (aProd != NULL) {
					if (aProd->nombre == cProd)
						break;
					if (aProd->next == NULL) {
						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (found) {
					SetWindowText(hTxtNombreProd, aProd->nombre.c_str());
					SetWindowText(hTxtCantidadProd, to_string(aProd->inventario).c_str());
					SetWindowText(hTxtPrecioProd, to_string(aProd->precio).c_str());
					SetWindowText(hTxtCodigoProd, to_string(aProd->codigo).c_str());
					SetWindowText(hTxtMarcaProd, aProd->marca.c_str());
					SetWindowText(hTxtDescProd, aProd->desc.c_str());
					SetWindowText(hTxtFotoProd1, aProd->foto1);
					SetWindowText(hTxtFotoProd2, aProd->foto2);

					HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aProd->foto1, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					HWND hPbImagen = GetDlgItem(hwnd, PC_CPFOTO1);
					SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
					SetWindowText(hTxtFotoProd1, aProd->foto1);

					hFoto = (HBITMAP)LoadImage(NULL, aProd->foto2, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
					hPbImagen = GetDlgItem(hwnd, PC_CPFOTO2);
					SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
					SetWindowText(hTxtFotoProd2, aProd->foto2);
				}
				aProd = oProd;
			}
		}break;

		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadProd();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadProd();
		}break;

		case ID_REGRESAR_MEN40018: {
			freeMemory();
			EndDialog(hwnd, 0);
		}break;

		case BTN_CPCAMBIO: {
			bool validar = true;
			char cProd[50];
			int selIndex = (int)SendMessage(hCB_BuscarProd, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_BuscarProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

			char txtNombreProd[50];
			char txtCantidadProd[50];
			char txtPrecioProd[50];
			char txtCodigoProd[50];
			char txtMarcaProd[50];
			char txtDescProd[50];
			char txtFotoProd1[50];
			char txtFotoProd2[50];

			int txtLengthNombre = GetWindowTextLength(hTxtNombreProd);
			int txtLengthCantidad = GetWindowTextLength(hTxtCantidadProd);
			int txtLengthPrecio = GetWindowTextLength(hTxtPrecioProd);
			int txtLengthCodigo = GetWindowTextLength(hTxtCodigoProd);
			int txtLengthMarca = GetWindowTextLength(hTxtMarcaProd);
			int txtLengthDesc = GetWindowTextLength(hTxtDescProd);
			int txtLengthFoto1 = GetWindowTextLength(hTxtFotoProd1);
			int txtLengthFoto2 = GetWindowTextLength(hTxtFotoProd2);

			if (txtLengthNombre < 1 || txtLengthCantidad < 1 || txtLengthPrecio < 1 || txtLengthCodigo < 1 || txtLengthMarca < 1 || txtLengthDesc < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			GetWindowText(hTxtNombreProd, txtNombreProd, ++txtLengthNombre);
			string sNombreP(txtNombreProd);
			GetWindowText(hTxtCantidadProd, txtCantidadProd, ++txtLengthCantidad);
			string sCantidadP(txtCantidadProd);
			GetWindowText(hTxtPrecioProd, txtPrecioProd, ++txtLengthPrecio);
			string sPrecioP(txtPrecioProd);
			GetWindowText(hTxtCodigoProd, txtCodigoProd, ++txtLengthCodigo);
			string sCodigoP(txtCodigoProd);
			GetWindowText(hTxtMarcaProd, txtMarcaProd, ++txtLengthMarca);
			string sMarcaP(txtMarcaProd);
			GetWindowText(hTxtDescProd, txtDescProd, ++txtLengthDesc);
			string sDescP(txtDescProd);
			GetWindowText(hTxtFotoProd1, txtFotoProd1, ++txtLengthFoto1);
			GetWindowText(hTxtFotoProd2, txtFotoProd2, ++txtLengthFoto2);

			if (oProd != NULL) {
				bool found = true;
				while (aProd->nombre.compare(sNombreP) != 0) {
					if (aProd->next == NULL) {
						found = false;
						break;
					}
					aProd = aProd->next;
				}
				if (found) {
					if (aProd->nombre != cProd){
						validar = false;
						MessageBox(hwnd, "Nombre repetido", "NO REGISTRO", MB_ICONERROR);
						break;
					}
				}
				aProd = oProd;
			}

			if (txtFotoProd1[0] == NULL || txtFotoProd2[0] == NULL) {
				validar = false;
				MessageBox(hwnd, "Debes insertar dos fotos", "NO GUARDADO", MB_ICONERROR);
			}

			for (int i = 0; i < sCantidadP.length(); i++) {
				bool esNum = sCantidadP[i] >= '0' && sCantidadP[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Cantidad\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sPrecioP.length(); i++) {
				bool esNum = sPrecioP[i] >= '0' && sPrecioP[i] <= '9' || sPrecioP[i] == '.';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Precio\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sCodigoP.length(); i++) {
				bool esNum = sCodigoP[i] >= '0' && sCodigoP[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Código\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
				else {
					if (oProd != NULL) {
						bool found = true;
						while (aProd->codigo != stoi(sCodigoP)) {
							if (aProd->next == NULL) {
								found = false;
								break;
							}
							aProd = aProd->next;
						}
						if (found) {
							if (aProd->nombre == cProd)
								break;
							else{
							validar = false;
							MessageBox(hwnd, "Código repetido", "NO SE DIO DE ALTA", MB_ICONERROR);
							break;
							}
						}
						aProd = oProd;
					}

				}
			}
			
			while (aProd != NULL) {
				if (aProd->nombre == cProd)
					break;
				if (aProd->next == NULL) {
					validar = false;
					MessageBox(hwnd, "Fallo", "NO GUARDADO", MB_ICONERROR);
					break;
				}
				aProd = aProd->next;
			}


			if (validar == true) {
				aProd->nombre = sNombreP;
				aProd->inventario = stoi(sCantidadP);
				aProd->precio = stof(sPrecioP);
				aProd->codigo = stoi(sCodigoP);
				aProd->marca = sMarcaP;
				aProd->desc = sDescP;
				strcpy(aProd->foto1, txtFotoProd1);
				strcpy(aProd->foto2, txtFotoProd2);
				aProd = oProd;

				saveProd(aProd);
				aProd = oProd;
				freeMemory();
				EndDialog(hwnd, 1);

			}
		}break;

		}
	}break;

	case WM_CLOSE:
		break;
	case WM_DESTROY: {
	}break;

	}return FALSE;
}

BOOL CALLBACK fBorrarProd(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadProd();
		loadEnvio();
		hCB_MBuscarProd = GetDlgItem(hwnd, CB_BPPRODUCTO);
		while (aProd != NULL) {
			if (loggedUser == aProd->IDUser) {
				SendMessage(hCB_MBuscarProd, CB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
			}
			aProd = aProd->next;
		}
		aProd = oProd;

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_BP);
		HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_BP);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		LB_BorrarProd = GetDlgItem(hwnd, LB_BPDATOS);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadProd();
			loadEnvio();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadProd();
			loadEnvio();
		}break;

		case ID_REGRESAR_MEN40018: {
			freeMemory();
			EndDialog(hwnd, 0);
		}break;

		case CB_BPPRODUCTO: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				SendMessage(LB_BorrarProd, LB_RESETCONTENT, 0, 0);
				char cProd[50];

				int selIndex = (int)SendMessage(hCB_MBuscarProd, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_MBuscarProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);
				string sProd(cProd);

				bool found = true;
				while (aProd != NULL) {
					if (aProd->nombre.compare(sProd) == 0)
						break;
					if (aProd->next == NULL) {
						found = false;
						MessageBox(hwnd, "ERROR", "productos", MB_ICONEXCLAMATION);
						break;
					}
					aProd = aProd->next;
				}

				if (found) {
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)"Cantidad:");
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)to_string(aProd->inventario).c_str());
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)"Precio:");
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)to_string(aProd->precio).c_str());
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)"Código:");
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)to_string(aProd->codigo).c_str());
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)"Marca:");
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)aProd->marca.c_str());
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)"Descripción:");
					SendMessage(LB_BorrarProd, LB_ADDSTRING, NULL, (LPARAM)aProd->desc.c_str());

					aProd = oProd;
					aEnvio = oEnvio;
				}
			}
		}break;

		case BTN_BPBAJA: {
			bool validar = true;
			char cProd[50];
			int indiceProd = (int)SendMessage(hCB_MBuscarProd, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_MBuscarProd, CB_GETLBTEXT, (WPARAM)indiceProd, (LPARAM)cProd);
			string sProd(cProd);
			while (aProd != NULL) {
				if (aProd->nombre.compare(sProd) == 0)
					break;
				if (aProd->next == NULL) {
					validar = false;
					MessageBox(hwnd, "ERROR", "Validar guardar prod", MB_ICONEXCLAMATION);
					break;
				}
				aProd = aProd->next;
			}

			while (aEnvio != NULL) {
				if (aProd->IDProd == aEnvio->IDProd) {
					validar = false;
					MessageBox(hwnd, "No puedes borrar un producto que esta asignado a un envio \nAsegurate de borrar primero el envio", "NO BORRADO", MB_ICONEXCLAMATION);
				}

				if (aEnvio->next == NULL) {
					break;
				}
				aEnvio = aEnvio->next;
			}



			if (validar == true) {
				if (aProd->prev == NULL && aProd->next == NULL) {
					delete aProd;
					aProd = oProd = NULL;
					GLOBAL_PRODUCTO_ID = 1;
				}
				else if (aProd->prev == NULL) {
					oProd = oProd->next;
					oProd->prev = NULL;
					delete aProd;
					aProd = oProd;
					GLOBAL_PRODUCTO_ID = 1;
					while (aProd != NULL) {
						aProd->IDProd = GLOBAL_PRODUCTO_ID++;
						aProd = aProd->next;
					}
					aProd = oProd;
				}
				else if (aProd->next == NULL) {
					aProd->prev->next = NULL;
					delete aProd;
					aProd = oProd;
					GLOBAL_PRODUCTO_ID = GLOBAL_PRODUCTO_ID - 1;
				}
				else {
					aProd->prev->next = aProd->next;
					aProd->next->prev = aProd->prev;
					delete aProd;
					aProd = oProd;
					GLOBAL_PRODUCTO_ID = 1;
					while (aProd != NULL) {
						aProd->IDProd = GLOBAL_PRODUCTO_ID++;
						aProd = aProd->next;
					}
					aProd = oProd;
				}
				saveGlobalId();
				saveProd(aProd);
				aProd = oProd;
				freeMemory();
				EndDialog(hwnd, 1);
				MessageBox(NULL, "EXITOSO!", "BORRADO", MB_ICONINFORMATION);
			}
			aProd = oProd;
			
		}break;

		}
	}break;

	case WM_CLOSE:
		break;

	case WM_DESTROY: {
	}break;

	}return FALSE;
}

BOOL CALLBACK fAltaEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_AE);
		HBITMAP hFotoPerfil = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE); 
		HWND hPbImagen = GetDlgItem(hwnd, PC_AE);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFotoPerfil);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		loadEnvio();
		loadProd();
		hCB_EnvioProd = GetDlgItem(hwnd, CB_APRODUCTO);
		hTxtCantidadEnvio = GetDlgItem(hwnd, EB_ACANTIDAD);
		hTxtPrecioEnvio = GetDlgItem(hwnd, EB_APRECIO);
		hTxtCalleEnvio = GetDlgItem(hwnd, EB_ACALLE);
		hTxtColoniaEnvio = GetDlgItem(hwnd, EB_ACOLONIA);
		hTxtCiudadEnvio = GetDlgItem(hwnd, EB_ACIUDAD);
		hTxtEstadoEnvio = GetDlgItem(hwnd, EB_AESTADO);
		hTxtMensajeEnvio = GetDlgItem(hwnd, EB_AMENSAJE);
		hDTCFechaEnvio = GetDlgItem(hwnd, DTC_AFECHAENVIO);

		while (aProd != NULL) {
			if (loggedUser == aProd->IDUser) {
				SendMessage(hCB_EnvioProd, CB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
			}
			aProd = aProd->next;
		}
		aProd = oProd;
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadEnvio();
			loadProd();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadEnvio();
			loadProd();
		}break;

		case ID_REGRESAR_MEN40018: {
			freeMemory();
			exitWindow = true;
			DestroyWindow(hwnd);
		}break;

		case EB_ACANTIDAD: {
			if (HIWORD(wparam) == EN_CHANGE) {
				bool validar = true;
				char txtCantidadProd[50];
				int txtLengthCantidad = GetWindowTextLength(hTxtCantidadEnvio);

				if (txtLengthCantidad != 0) {
					GetWindowText(hTxtCantidadEnvio, txtCantidadProd, ++txtLengthCantidad);
					string sCantidadP(txtCantidadProd);

					for (int i = 0; i < sCantidadP.length(); i++) {
						bool esNum = sCantidadP[i] >= '0' && sCantidadP[i] <= '9';
						if (!esNum) {
							SetWindowText(hTxtPrecioEnvio, "Error");
							MessageBox(hwnd, "Cantidad\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
							validar = false;
							break;
						}
					}
					if (validar == true) {
						char cProd[50];

						int selIndex = (int)SendMessage(hCB_EnvioProd, CB_GETCURSEL, NULL, NULL);
						SendMessage(hCB_EnvioProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

						bool found = true;
						while (aProd != NULL) {
							if (aProd->nombre == cProd)
								break;
							if (aProd->next == NULL) {
								found = false;
								break;
							}
							aProd = aProd->next;
						}
						if (found)
							SetWindowText(hTxtPrecioEnvio, to_string(stoi(sCantidadP) * aProd->precio).c_str());
						aProd = oProd;
					}
				}
			}
		}break;

		case CB_APRODUCTO: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hwnd, EB_ACANTIDAD), 1);
				SetWindowText(hTxtCantidadEnvio, to_string(0).c_str());
			}
		}break;

		case BTN_AREGISTRAR: {
			bool validar = true;

			char txtCantidadEnvio[50];
			char txtCalleEnvio[50];
			char txtColoniaEnvio[50];
			char txtCiudadEnvio[50];
			char txtEstadoEnvio[50];
			char txtMensajeEnvio[50];
			char txtFechaEnvio[50];

			int txtLengthCantidad = GetWindowTextLength(hTxtCantidadEnvio);
			int txtLengthCalle = GetWindowTextLength(hTxtCalleEnvio);
			int txtLengthColonia = GetWindowTextLength(hTxtColoniaEnvio);
			int txtLengthCiudad = GetWindowTextLength(hTxtCiudadEnvio);
			int txtLengthEstado = GetWindowTextLength(hTxtEstadoEnvio);
			int txtLengthMensaje = GetWindowTextLength(hTxtMensajeEnvio);
			int txtLengthFecha = GetWindowTextLength(hDTCFechaEnvio);

			time(&actualTime);
			timeInfo = localtime(&actualTime);

			char cProd[50];
			int selIndex = (int)SendMessage(hCB_EnvioProd, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_EnvioProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

			if (selIndex < 0) {
				MessageBox(hwnd, "Debes seleccionar un producto", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			if (txtLengthCantidad < 1 || txtLengthCalle < 1 || txtLengthColonia < 1 ||
				txtLengthCiudad < 1 || txtLengthEstado < 1 || txtLengthMensaje < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			GetWindowText(hTxtCantidadEnvio, txtCantidadEnvio, ++txtLengthCantidad);
			string sCantidadE(txtCantidadEnvio);
			GetWindowText(hTxtCalleEnvio, txtCalleEnvio, ++txtLengthCalle);
			string sCalleE(txtCalleEnvio);
			GetWindowText(hTxtColoniaEnvio, txtColoniaEnvio, ++txtLengthColonia);
			string sColoniaE(txtColoniaEnvio);
			GetWindowText(hTxtCiudadEnvio, txtCiudadEnvio, ++txtLengthCiudad);
			string sCiudadE(txtCiudadEnvio);
			GetWindowText(hTxtEstadoEnvio, txtEstadoEnvio, ++txtLengthEstado);
			string sEstadoE(txtEstadoEnvio);
			GetWindowText(hTxtMensajeEnvio, txtMensajeEnvio, ++txtLengthMensaje);
			string sMensajeE(txtMensajeEnvio);
			GetWindowText(hDTCFechaEnvio, txtFechaEnvio, ++txtLengthFecha);
			string sFechaE(txtFechaEnvio);
			string dd=sFechaE.substr(0,2);
			string mm=sFechaE.substr(3,2);
			string yyyy=sFechaE.substr(6,4);
			string lol= "/";
			sFechaE=yyyy+lol+mm+lol+dd;
			bool fecha = true;
			if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(0, 4)) && (timeInfo->tm_mon + 1) == stoi(sFechaE.substr(5, 2)) && timeInfo->tm_mday < stoi(sFechaE.substr(8, 2)));
			else if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(0, 4)) && (timeInfo->tm_mon + 1) < stoi(sFechaE.substr(5, 2)));
			else if ((timeInfo->tm_year + 1900) < stoi(sFechaE.substr(0, 4)));
			else
			
				fecha = false;

			if (oEnvio != NULL) {
				bool found = true;
				while (aEnvio->fecha.compare(sFechaE) != 0) {
					if (aEnvio->next == NULL) {
						found = false;
						break;
					}
					aEnvio = aEnvio->next;
				}
				aEnvio = oEnvio;
				if (found) {
					validar = false;
					MessageBox(hwnd, "Fecha ya reservada", "NO REGISTRO", MB_ICONERROR);
					break;
				}
			}

			for (int i = 0; i < sCantidadE.length(); i++) {
				bool esNum = sCantidadE[i] >= '0' && sCantidadE[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Cantidad\nTiene que ser texto numérico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}
			if (stoi(sCantidadE) == 0) {
				validar = false;
				MessageBox(hwnd, "Cantidad\nTiene que ser mayor a 0", "NO SE DIO DE ALTA", MB_ICONERROR);
				break;
			}
		
			int IDprod;
			bool found = true;
			while (aProd != NULL) {
				if (aProd->nombre == cProd)
					break;
				if (aProd->next == NULL) {
					found = false;
					break;
				}
				aProd = aProd->next;
			}

			if (found)
				IDprod = aProd->IDProd;

			if (aProd->inventario < stoi(sCantidadE)) {
				validar = false;
				MessageBox(hwnd, "Cantidad\nNo puede ser mayor a lo que hay en inventario", "NO SE DIO DE ALTA", MB_ICONERROR);
				break;
			}
			if (aProd->inventario >= stoi(sCantidadE))
				aProd->inventario = aProd->inventario - stoi(sCantidadE);
			aProd = oProd;

			if(validar==true){			
				if (oEnvio == NULL) {
					oEnvio = new Envios;
					oEnvio->IDUser = loggedUser;
					oEnvio->IDProd = IDprod;
					oEnvio->IDEnvio = GLOBAL_ENVIO_ID++;
					oEnvio->cantidad = stoi(sCantidadE);
					oEnvio->calle = sCalleE;
					oEnvio->colonia = sColoniaE;
					oEnvio->ciudad = sCiudadE;
					oEnvio->estado = sEstadoE;
					oEnvio->mensaje = sMensajeE;
					oEnvio->fecha = sFechaE;
					oEnvio->prev = NULL;
					oEnvio->next = NULL;
					aEnvio = oEnvio;
				}
				else {
					while (aEnvio->next != NULL)
						aEnvio = aEnvio->next;
					aEnvio->next = new Envios;
					aEnvio->next->prev = aEnvio;
					aEnvio = aEnvio->next;
					aEnvio->IDUser = loggedUser;
					aEnvio->IDProd = IDprod;
					aEnvio->IDEnvio = GLOBAL_ENVIO_ID++;
					aEnvio->cantidad = stoi(sCantidadE);
					aEnvio->calle = sCalleE;
					aEnvio->colonia = sColoniaE;
					aEnvio->ciudad = sCiudadE;
					aEnvio->estado = sEstadoE;
					aEnvio->mensaje = sMensajeE;
					aEnvio->fecha = sFechaE;
					aEnvio->next = NULL;
					aEnvio = oEnvio;
				}

				saveGlobalId();
				saveProd(aProd);
				aProd = oProd;
				saveEnvio(aEnvio);
				aEnvio = oEnvio;
				freeMemory();
				exitWindow = true;
				DestroyWindow(hwnd);
			}
		}break;

		}break;
	}break;

	case WM_CLOSE: {

	}break;

	case WM_DESTROY: {
		if (exitWindow) {
			HWND hPrincipal = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_PRINCIPAL), NULL, fPrincipal);
			ShowWindow(hPrincipal, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}
	}break;

	}return FALSE;
}

BOOL CALLBACK fMostrarEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		menu3 = GetMenu(hwnd);
		EnableMenuItem(menu3, ID_INFORMACI40016, 0x00000001L);
		EnableMenuItem(menu3, ID_INFORMACI40017, 0x00000001L);

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_ME);
		HBITMAP hFotoPerfil = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_ME);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFotoPerfil);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		loadEnvio();
		loadProd();

		time(&actualTime);
		timeInfo = localtime(&actualTime);

		hCB_MBuscarEnvio = GetDlgItem(hwnd, CB_MEPRODUCTO);
		while (aEnvio != NULL) {
			if (loggedUser == aEnvio->IDUser)
				SendMessage(hCB_MBuscarEnvio, CB_ADDSTRING, NULL, (LPARAM)aEnvio->fecha.c_str());

			aEnvio = aEnvio->next;
		}
		aEnvio = oEnvio;

		hTxtMEnvioProd = GetDlgItem(hwnd, EB_MEPROD);
		hTxtMCantidadEnvio = GetDlgItem(hwnd, EB_MECANTIDAD);
		hTxtMPrecioEnvio = GetDlgItem(hwnd, EB_MEPRECIO);
		hTxtMCalleEnvio = GetDlgItem(hwnd, EB_MECALLE);
		hTxtMColoniaEnvio = GetDlgItem(hwnd, EB_MECOLONIA);
		hTxtMCiudadEnvio = GetDlgItem(hwnd, EB_MECIUDAD);
		hTxtMEstadoEnvio = GetDlgItem(hwnd, EB_MESTADO);
		hTxtMMensajeEnvio = GetDlgItem(hwnd, EB_MEMENSAJE);
		hTxtMFechaEnvio = GetDlgItem(hwnd, EB_MFECHA);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_REGRESAR_MEN40018: {
			aEnvio = oEnvio;
			EndDialog(hwnd, 0);
			freeMemory();
		}break;

		case CB_MEPRODUCTO: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				char cEnvio[50];

				int selIndex = (int)SendMessage(hCB_MBuscarEnvio, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_MBuscarEnvio, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cEnvio);
				string sEnvio(cEnvio);
				bool found = true;
				while (aEnvio != NULL) {
					if (aEnvio->fecha.compare( sEnvio)==0 )
						break;
					if (aEnvio->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "ENVIOS", MB_ICONEXCLAMATION);
						break;
					}
					aEnvio = aEnvio->next;
				}

				string selectProd;
				float selPrecio;
				while (aProd != NULL) {
					if (aProd->IDProd == aEnvio->IDProd) {
						selectProd = aProd->nombre;
						selPrecio = aProd->precio;
						break;
					}

					if (aProd->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "Comparar", MB_ICONEXCLAMATION);
						break;
					}
					aProd = aProd->next;
				}
				aProd = oProd;			

				if (found) {
					SetWindowText(hTxtMEnvioProd, selectProd.c_str());
					SetWindowText(hTxtMCantidadEnvio, to_string(aEnvio->cantidad).c_str());
					SetWindowText(hTxtMPrecioEnvio, to_string(selPrecio * aEnvio->cantidad).c_str());
					SetWindowText(hTxtMCalleEnvio, aEnvio->calle.c_str());
					SetWindowText(hTxtMColoniaEnvio, aEnvio->colonia.c_str());
					SetWindowText(hTxtMCiudadEnvio, aEnvio->ciudad.c_str());
					SetWindowText(hTxtMEstadoEnvio, aEnvio->estado.c_str());
					SetWindowText(hTxtMMensajeEnvio, aEnvio->mensaje.c_str());
					SetWindowText(hTxtMFechaEnvio, aEnvio->fecha.c_str());
				}
				aProd = oProd;
				aEnvio = oEnvio;
			}
		}break;

		}break;
	}break;

	case WM_CLOSE:
		break;
	case WM_DESTROY:
		break;

	}return FALSE;
}

BOOL CALLBACK fCambioEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadEnvio();
		loadProd();

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_CE);
		HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_CE);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		time(&actualTime);
		timeInfo = localtime(&actualTime);

		MessageBox(hwnd, "Solo se podran modificar los envios que aun no han sido enviados", "ENVIOS", MB_ICONEXCLAMATION);

		hCB_BuscarEnvio = GetDlgItem(hwnd, IDC_CEBUSCAR);
		while (aEnvio != NULL) {
			if (loggedUser == aEnvio->IDUser) {
				bool fecha = true;
				if ((timeInfo->tm_year + 1900) == stoi(aEnvio->fecha.substr(0, 4)) && (timeInfo->tm_mon + 1) == stoi(aEnvio->fecha.substr(5, 2)) && timeInfo->tm_mday < stoi(aEnvio->fecha.substr(8, 2)));
				else if ((timeInfo->tm_year + 1900) == stoi(aEnvio->fecha.substr(0, 4)) && (timeInfo->tm_mon + 1) < stoi(aEnvio->fecha.substr(5, 2)));
				else if ((timeInfo->tm_year + 1900) < stoi(aEnvio->fecha.substr(0, 4)));
				else
					fecha = false;

				if (fecha == true)
					SendMessage(hCB_BuscarEnvio, CB_ADDSTRING, NULL, (LPARAM)aEnvio->fecha.c_str());

			}
			aEnvio = aEnvio->next;
		}
		aEnvio = oEnvio;

		hCB_EnvioProd = GetDlgItem(hwnd, CB_CEPRODUCTO);
		while (aProd != NULL) {
			if (loggedUser == aProd->IDUser) {
				SendMessage(hCB_EnvioProd, CB_ADDSTRING, NULL, (LPARAM)aProd->nombre.c_str());
			}
			aProd = aProd->next;
		}
		aProd = oProd;

		hTxtCantidadEnvio = GetDlgItem(hwnd, EB_CECANTIDAD);
		hTxtPrecioEnvio = GetDlgItem(hwnd, EB_CEPRECIO);
		hTxtCalleEnvio = GetDlgItem(hwnd, EB_CECALLE);
		hTxtColoniaEnvio = GetDlgItem(hwnd, EB_CECOLONIA);
		hTxtCiudadEnvio = GetDlgItem(hwnd, EB_CECIUDAD);
		hTxtEstadoEnvio = GetDlgItem(hwnd, EB_CEESTADO);
		hTxtMensajeEnvio = GetDlgItem(hwnd, EB_CEMENSAJE);
		hDTCFechaEnvio = GetDlgItem(hwnd, DTC_CEFECHAENVIO);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {

		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadEnvio();
			loadProd();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadEnvio();
			loadProd();
		}break;

		case ID_REGRESAR_MEN40018: {
			freeMemory();
			exitWindow = true;
			DestroyWindow(hwnd);
		}break;

		case EB_CECANTIDAD: {
			if (HIWORD(wparam) == EN_CHANGE) {
				bool validar = true;
				char txtCantidadProd[50];
				int txtLengthCantidad = GetWindowTextLength(hTxtCantidadEnvio);

				if (txtLengthCantidad != 0) {
					GetWindowText(hTxtCantidadEnvio, txtCantidadProd, ++txtLengthCantidad);
					string sCantidadP(txtCantidadProd);

					for (int i = 0; i < sCantidadP.length(); i++) {
						bool esNum = sCantidadP[i] >= '0' && sCantidadP[i] <= '9';
						if (!esNum) {
							SetWindowText(hTxtPrecioEnvio, "Error");
							MessageBox(hwnd, "Cantidad\nTiene que ser texto numerico", "NO SE DIO DE ALTA", MB_ICONERROR);
							validar = false;
							break;
						}
					}
					if (validar == true) {
						char cProd[50];

						int selIndex = (int)SendMessage(hCB_EnvioProd, CB_GETCURSEL, NULL, NULL);
						SendMessage(hCB_EnvioProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);

						bool found = true;
						while (aProd != NULL) {
							if (aProd->nombre == cProd)
								break;
							if (aProd->next == NULL) {
								found = false;
								break;
							}
							aProd = aProd->next;
						}
						if (found)
							SetWindowText(hTxtPrecioEnvio, to_string(stoi(sCantidadP) * aProd->precio).c_str());
						aProd = oProd;
					}
				}
			}
		}break;

		case IDC_CEBUSCAR: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				EnableWindow(GetDlgItem(hwnd, CB_CEPRODUCTO), 1);
				EnableWindow(GetDlgItem(hwnd, BTN_CECAMBIAR), 1);
				EnableWindow(GetDlgItem(hwnd, EB_CECANTIDAD), 1);
				char cEnvio[50];

				int selIndex = (int)SendMessage(hCB_BuscarEnvio, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_BuscarEnvio, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cEnvio);

				bool found = true;
				while (aEnvio != NULL) {
					if (aEnvio->fecha == cEnvio && selIndex < aEnvio->IDEnvio)
						break;
					if (aEnvio->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "ENVIOS", MB_ICONEXCLAMATION);
						break;
					}
					aEnvio = aEnvio->next;
				}

				string selectProd;
				float selPrecio;
				while (aProd != NULL) {
					if (aProd->IDProd == aEnvio->IDProd) {
						selectProd = aProd->nombre;
						selPrecio = aProd->precio;
						break;
					}

					if (aProd->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "Comparar", MB_ICONEXCLAMATION);
						break;
					}
					aProd = aProd->next;
				}
				aProd = oProd;

				int indice = 0;
				char cComparar[50];
				while (aProd != NULL) {
					SendMessage(hCB_EnvioProd, CB_GETLBTEXT, (WPARAM)indice, (LPARAM)cComparar);
					string sComparar(cComparar);

					if (selectProd.compare(sComparar) == 0)
						break;

					if (aProd->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "buscar indice", MB_ICONEXCLAMATION);
						break;
					}

					aProd = aProd->next;
					indice = indice + 1;
				}
				aProd = oProd;


				if (found) {

					SendMessage(hCB_EnvioProd, CB_SETCURSEL, indice, NULL);

					SetWindowText(hTxtCantidadEnvio, to_string(aEnvio->cantidad).c_str());
					SetWindowText(hTxtPrecioEnvio, to_string(selPrecio* aEnvio->cantidad).c_str());
					SetWindowText(hTxtCalleEnvio, aEnvio->calle.c_str());
					SetWindowText(hTxtColoniaEnvio, aEnvio->colonia.c_str());
					SetWindowText(hTxtCiudadEnvio, aEnvio->ciudad.c_str());
					SetWindowText(hTxtEstadoEnvio, aEnvio->estado.c_str());
					SetWindowText(hTxtMensajeEnvio, aEnvio->mensaje.c_str());
					SetWindowText(hDTCFechaEnvio, aEnvio->fecha.c_str());
				}
				aProd = oProd;
				aEnvio = oEnvio;
			}
		}break;

		case CB_CEPRODUCTO: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				SetWindowText(hTxtCantidadEnvio, to_string(0).c_str());
			}
		}break;

		case BTN_CECAMBIAR: {
			bool validar = true;

			char txtCantidadEnvio[50];
			char txtCalleEnvio[50];
			char txtColoniaEnvio[50];
			char txtCiudadEnvio[50];
			char txtEstadoEnvio[50];
			char txtMensajeEnvio[50];
			char txtFechaEnvio[50];

			int txtLengthCantidad = GetWindowTextLength(hTxtCantidadEnvio);
			int txtLengthCalle = GetWindowTextLength(hTxtCalleEnvio);
			int txtLengthColonia = GetWindowTextLength(hTxtColoniaEnvio);
			int txtLengthCiudad = GetWindowTextLength(hTxtCiudadEnvio);
			int txtLengthEstado = GetWindowTextLength(hTxtEstadoEnvio);
			int txtLengthMensaje = GetWindowTextLength(hTxtMensajeEnvio);
			int txtLengthFecha = GetWindowTextLength(hDTCFechaEnvio);

			time(&actualTime);
			timeInfo = localtime(&actualTime);

			char cProd[50];
			int selIndex = (int)SendMessage(hCB_EnvioProd, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_EnvioProd, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cProd);


			if (selIndex < 0) {
				MessageBox(hwnd, "Debes seleccionar un producto", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			if (txtLengthCantidad < 1 || txtLengthCalle < 1 || txtLengthColonia < 1 || txtLengthCiudad < 1 || txtLengthEstado < 1 || txtLengthMensaje < 1) {
				MessageBox(hwnd, "Falta llenar información", "NO SE DIO DE ALTA", MB_ICONERROR);
				validar = false;
				break;
			}

			GetWindowText(hTxtCantidadEnvio, txtCantidadEnvio, ++txtLengthCantidad);
			string sCantidadE(txtCantidadEnvio);
			GetWindowText(hTxtCalleEnvio, txtCalleEnvio, ++txtLengthCalle);
			string sCalleE(txtCalleEnvio);
			GetWindowText(hTxtColoniaEnvio, txtColoniaEnvio, ++txtLengthColonia);
			string sColoniaE(txtColoniaEnvio);
			GetWindowText(hTxtCiudadEnvio, txtCiudadEnvio, ++txtLengthCiudad);
			string sCiudadE(txtCiudadEnvio);
			GetWindowText(hTxtEstadoEnvio, txtEstadoEnvio, ++txtLengthEstado);
			string sEstadoE(txtEstadoEnvio);
			GetWindowText(hTxtMensajeEnvio, txtMensajeEnvio, ++txtLengthMensaje);
			string sMensajeE(txtMensajeEnvio);
			GetWindowText(hDTCFechaEnvio, txtFechaEnvio, ++txtLengthFecha);
			string sFechaE(txtFechaEnvio);
			string dd = sFechaE.substr(0, 2);
			string mm = sFechaE.substr(3, 2);
			string yyyy = sFechaE.substr(6, 4);
			string lol = "/";
			sFechaE = yyyy + lol + mm + lol + dd;
			bool fecha = true;
			if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(0, 4)) && (timeInfo->tm_mon + 1) == stoi(sFechaE.substr(5, 2)) && timeInfo->tm_mday < stoi(sFechaE.substr(8, 2)));
			else if ((timeInfo->tm_year + 1900) == stoi(sFechaE.substr(0, 4)) && (timeInfo->tm_mon + 1) < stoi(sFechaE.substr(5, 2)));
			else if ((timeInfo->tm_year + 1900) < stoi(sFechaE.substr(0, 4)));
			else
				fecha = false;

			if (fecha == false) {
				validar = false;
				MessageBox(hwnd, "Fecha\nSolo se pueden seleccionar fechas a futuro", "NO SE DIO DE ALTA", MB_ICONERROR);
				break;
			}
			for (int i = 0; i < sCantidadE.length(); i++) {
				bool esNum = sCantidadE[i] >= '0' && sCantidadE[i] <= '9';
				if (!esNum) {
					validar = false;
					MessageBox(hwnd, "Cantidad\nTiene que ser texto numérico", "NO SE DIO DE ALTA", MB_ICONERROR);
					break;
				}
			}
			if (stoi(sCantidadE) == 0) {
				validar = false;
				MessageBox(hwnd, "Cantidad\nTiene que ser mayor a 0", "NO SE DIO DE ALTA", MB_ICONERROR);
				break;
			}

			int IDprod;
			bool found = true;
			while (aProd != NULL) {
				if (aProd->nombre == cProd)
					break;
				if (aProd->next == NULL) {
					found = false;
					break;
				}
				aProd = aProd->next;
			}

			if (found)
				IDprod = aProd->IDProd;

			aProd->inventario = aProd->inventario + aEnvio->cantidad;
			if (aProd->inventario < stoi(sCantidadE)) {
				validar = false;
				MessageBox(hwnd, "Cantidad\nNo puede ser mayor a lo que hay en inventario", "NO SE DIO DE ALTA", MB_ICONERROR);
				break;
			}
			
			if (aProd->inventario >= stoi(sCantidadE))
				aProd->inventario = aProd->inventario - stoi(sCantidadE);

			aProd = oProd;

			char cEnvio[50];
			int indiceEnvio = (int)SendMessage(hCB_BuscarEnvio, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_BuscarEnvio, CB_GETLBTEXT, (WPARAM)indiceEnvio, (LPARAM)cEnvio);
			string sEnvio(cEnvio);


			if (oEnvio != NULL) {
				bool found = true;
				while (aEnvio->fecha.compare(sFechaE) != 0) {
					if (aEnvio->next == NULL) {
						found = false;
						break;
					}
					aEnvio = aEnvio->next;
				}
				aEnvio = oEnvio;
				if (found) {
					validar = false;
					MessageBox(hwnd, "Fecha ya reservada", "NO REGISTRO", MB_ICONERROR);
					break;
				}
			}
			while (aEnvio != NULL) {
				if (aEnvio->fecha.compare(sEnvio) == 0)
					break;
				if (aEnvio->next == NULL) {					
					validar = false;
					MessageBox(hwnd, "Error", "Validar guardar envio", MB_ICONEXCLAMATION);
					break;
				}
				aEnvio = aEnvio->next;
			}

			if (validar == true) {
				aEnvio->IDProd = IDprod;
				aEnvio->cantidad = stoi(sCantidadE);
				aEnvio->calle = sCalleE;
				aEnvio->colonia = sColoniaE;
				aEnvio->ciudad = sCiudadE;
				aEnvio->estado = sEstadoE;
				aEnvio->mensaje = sMensajeE;
				aEnvio->fecha = sFechaE;
				aEnvio = oEnvio;
				saveEnvio(aEnvio);
				aEnvio = oEnvio;
				saveProd(aProd);
				aProd = oProd;
				freeMemory();

				exitWindow = true;
				DestroyWindow(hwnd);
			}
			freeMemory();
		}break;


		}
	}break;

	case WM_CLOSE:
		break;
	case WM_DESTROY: {
		if (exitWindow) {
			HWND hPrincipal = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_PRINCIPAL), NULL, fPrincipal);
			ShowWindow(hPrincipal, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}
	}break;

	}return FALSE;
}

BOOL CALLBACK fBorrarEnvio(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_INITDIALOG: {
		loadEnvio();
		loadProd();

		loadInfo();
		while (aInfo != NULL) {
			if (aInfo->IDUser == loggedUser)
				break;
			aInfo = aInfo->next;
		}
		hTxtNombre = GetDlgItem(hwnd, TC_BE);
		HBITMAP hFoto = (HBITMAP)LoadImage(NULL, aInfo->foto, IMAGE_BITMAP, 50, 50, LR_LOADFROMFILE);
		HWND hPbImagen = GetDlgItem(hwnd, PC_BE);
		SendMessage(hPbImagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hFoto);
		SetWindowText(hTxtNombre, aInfo->nombreUs.c_str());
		aInfo = oInfo;

		MessageBox(hwnd, "Solo se podran borrar los envios que ya fueron enviados", "ENVIOS", MB_ICONEXCLAMATION);

		hCB_BuscarEnvio = GetDlgItem(hwnd, CB_BEENVIO);
		while (aEnvio != NULL) {
			if (loggedUser == aEnvio->IDUser) {
				bool fecha = true;
				if ((timeInfo->tm_year + 1900) == stoi(aEnvio->fecha.substr(0, 4)) && (timeInfo->tm_mon + 1) == stoi(aEnvio->fecha.substr(5, 2)) && timeInfo->tm_mday < stoi(aEnvio->fecha.substr(8, 2)));
				else if ((timeInfo->tm_year + 1900) == stoi(aEnvio->fecha.substr(0, 4)) && (timeInfo->tm_mon + 1) < stoi(aEnvio->fecha.substr(5, 2)));
				else if ((timeInfo->tm_year + 1900) < stoi(aEnvio->fecha.substr(0, 4)));
				else
				SendMessage(hCB_BuscarEnvio, CB_ADDSTRING, NULL, (LPARAM)aEnvio->fecha.c_str());
			}
			aEnvio = aEnvio->next;
		}
		aEnvio = oEnvio;

		LB_BorrarEnvio = GetDlgItem(hwnd, LB_BEDATOS);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case ID_INFORMACI40016: {
			freeMemory();
			int hMostrarEnvio = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARENVIO), hwnd, fMostrarEnvio);
			loadEnvio();
			loadProd();
		}break;

		case ID_INFORMACI40017: {
			freeMemory();
			int hMostrarProd = DialogBox(hgInst, MAKEINTRESOURCE(DLG_MOSTRARPRODUCTO), hwnd, fMostrarProd);
			loadEnvio();
			loadProd();

		}break;

		case ID_REGRESAR_MEN40018: {
			exitWindow = true;
			DestroyWindow(hwnd);
		}break;

		case CB_BEENVIO: {
			if (HIWORD(wparam) == CBN_SELCHANGE) {
				SendMessage(LB_BorrarEnvio, LB_RESETCONTENT, 0, 0);
				char cEnvio[50];

				int selIndex = (int)SendMessage(hCB_BuscarEnvio, CB_GETCURSEL, NULL, NULL);
				SendMessage(hCB_BuscarEnvio, CB_GETLBTEXT, (WPARAM)selIndex, (LPARAM)cEnvio);
				string sEnvio(cEnvio);
				bool found = true;
				while (aEnvio != NULL) {
					if (aEnvio->fecha.compare(sEnvio) == 0 && selIndex < aEnvio->IDEnvio)
						break;
					if (aEnvio->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "ENVIOS", MB_ICONEXCLAMATION);
						break;
					}
					aEnvio = aEnvio->next;
				}

				string selectProd;
				float selPrecio;
				while (aProd != NULL) {
					if (aProd->IDProd == aEnvio->IDProd) {
						selectProd = aProd->nombre;
						selPrecio = aProd->precio;
						break;
					}

					if (aProd->next == NULL) {
						found = false;
						MessageBox(hwnd, "Error", "Comparar", MB_ICONEXCLAMATION);
						break;
					}
					aProd = aProd->next;
				}
				aProd = oProd;

				if (found) {
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->fecha.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Producto:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)selectProd.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Cantidad:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)to_string(aEnvio->cantidad).c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Precio:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)to_string(selPrecio).c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Monto X Producto:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)to_string(selPrecio * aEnvio->cantidad).c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Calle:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->calle.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Colonia:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->colonia.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Ciudad:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->ciudad.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Estado:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->estado.c_str());
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)"Mensaje:");
					SendMessage(LB_BorrarEnvio, LB_ADDSTRING, NULL, (LPARAM)aEnvio->mensaje.c_str());

					aProd = oProd;
					aEnvio = oEnvio;
				}
			}
		}break;

		case BTN_BEBAJA: {
			bool validar = true;
			char cEnvio[50];
			int indiceEnvio = (int)SendMessage(hCB_BuscarEnvio, CB_GETCURSEL, NULL, NULL);
			SendMessage(hCB_BuscarEnvio, CB_GETLBTEXT, (WPARAM)indiceEnvio, (LPARAM)cEnvio);
			string sEnvio(cEnvio);
			while (aEnvio != NULL) {
				if (aEnvio->fecha.compare(sEnvio) == 0)
					break;
				if (aEnvio->next == NULL) {
					validar = false;
					MessageBox(hwnd, "Error", "Validar guardar envio", MB_ICONEXCLAMATION);
					break;
				}
				aEnvio = aEnvio->next;
			}

			if (validar == true) {
				if (aEnvio->prev == NULL && aEnvio->next == NULL) {
					delete aEnvio;
					aEnvio = oEnvio = NULL;
					GLOBAL_ENVIO_ID = 1;
				}
				else if (aEnvio->prev == NULL) {
					oEnvio = oEnvio->next;
					oEnvio->prev = NULL;
					delete aEnvio;
					aEnvio = oEnvio;
					GLOBAL_ENVIO_ID = 1;
					while (aEnvio != NULL) {
						aEnvio->IDEnvio = GLOBAL_ENVIO_ID++;
						aEnvio = aEnvio->next;
					}
					aEnvio = oEnvio;
				}
				else if (aEnvio->next == NULL) {
					aEnvio->prev->next = NULL;
					delete aEnvio;
					aEnvio = oEnvio;
					GLOBAL_ENVIO_ID = GLOBAL_ENVIO_ID - 1;
				}
				else {
					aEnvio->prev->next = aEnvio->next;
					aEnvio->next->prev = aEnvio->prev;
					delete aEnvio;
					aEnvio = oEnvio;

					GLOBAL_ENVIO_ID = 1;
					while (aEnvio != NULL) {
						aEnvio->IDEnvio = GLOBAL_ENVIO_ID++;
						aEnvio = aEnvio->next;
					}
					aEnvio = oEnvio;
				}
				saveEnvio(aEnvio);
				aEnvio = oEnvio;
				saveGlobalId();
				exitWindow = true;
				MessageBox(NULL, "EXITOSO!", "BORRADO", MB_ICONINFORMATION);
				DestroyWindow(hwnd);				
			}			
			aEnvio = oEnvio;
		}break;
		}
	}break;

	case WM_CLOSE:
		break;

	case WM_DESTROY: {
		if (exitWindow) {
			freeMemory();
			HWND hPrincipal = CreateDialog(hgInst, MAKEINTRESOURCE(DLG_PRINCIPAL), NULL, fPrincipal);
			ShowWindow(hPrincipal, SW_SHOW);
			exitWindow = false;
			DestroyWindow(hwnd);
		}
	}break;

	}return FALSE;
}
#pragma endregion

#pragma region Funciones
void saveUser(User* pOrigen) {
	lectorEscritor.open("Usuarios.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(User));
			pOrigen = pOrigen->next;
		}
		lectorEscritor.close();
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONERROR);
		return;
	}
}

void loadUser() {
	lectorEscritor.open("Usuarios.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo vacio", "NO CARGAR", MB_ICONERROR);
			return;
		}
		for (int i = 0; i < totalChar / sizeof(User); i++) {
			if (oUser == NULL) {
				User* temp = new User;
				oUser = new User;
				lectorEscritor.seekg(i * sizeof(User));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				oUser->username.append(temp->username);
				oUser->password.append(temp->password);
				oUser->IDUser = temp->IDUser;
				oUser->prev = NULL;
				oUser->next = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aUser->next != NULL)
					aUser = aUser->next;
				User* temp = new User;
				aUser->next = new User;
				lectorEscritor.seekg(i * sizeof(User));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(User));
				aUser->next->prev = aUser;
				aUser = aUser->next;
				aUser->username.append(temp->username);
				aUser->password.append(temp->password);
				aUser->IDUser = temp->IDUser;
				aUser->next = NULL;
				aUser = oUser;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		return;
	}
	else {
		return;
	}
}

void saveInfo(InfoVendedor* pOrigen) {
	lectorEscritor.open("InfoVendedor.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(InfoVendedor));
			pOrigen = pOrigen->next;
		}
		lectorEscritor.close();
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONERROR);
		return;
	}
}

void loadInfo() {
	lectorEscritor.open("InfoVendedor.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo vacio", "NO CARGAR", MB_ICONERROR);
			return;
		}
		for (int i = 0; i < totalChar / sizeof(InfoVendedor); i++) {
			if (oInfo == NULL) {
				InfoVendedor* temp = new InfoVendedor;
				oInfo = new InfoVendedor;
				lectorEscritor.seekg(i * sizeof(InfoVendedor));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(InfoVendedor));
				oInfo->nombreUs.append(temp->nombreUs);
				oInfo->alias.append(temp->alias);
				strcpy(oInfo->foto, temp->foto);
				oInfo->IDUser = temp->IDUser;
				oInfo->prev = NULL;
				oInfo->next = NULL;
				aInfo = oInfo;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aInfo->next != NULL)
					aInfo = aInfo->next;
				InfoVendedor* temp = new InfoVendedor;
				aInfo->next = new InfoVendedor;
				lectorEscritor.seekg(i * sizeof(InfoVendedor));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(InfoVendedor));
				aInfo->next->prev = aInfo;
				aInfo = aInfo->next;
				aInfo->nombreUs.append(temp->nombreUs);
				aInfo->alias.append(temp->alias);
				strcpy(aInfo->foto, temp->foto);
				aInfo->IDUser = temp->IDUser;
				aInfo->next = NULL;
				aInfo = oInfo;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		return;
	}
	else {
		return;
	}
}

void saveProd(Productos* pOrigen) {
	lectorEscritor.open("Productos.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Productos));
			pOrigen = pOrigen->next;
		}
		lectorEscritor.close();
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONERROR);
		return;
	}
}

void loadProd() {
	lectorEscritor.open("Productos.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo vacio", "NO CARGAR", MB_ICONERROR);
			return;
		}
		for (int i = 0; i < totalChar / sizeof(Productos); i++) {
			if (oProd == NULL) {
				Productos* temp = new Productos;
				oProd = new Productos;
				lectorEscritor.seekg(i * sizeof(Productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Productos));
				oProd->IDUser = temp->IDUser;
				oProd->IDProd = temp->IDProd;
				oProd->nombre.append(temp->nombre);
				oProd->inventario = temp->inventario;
				oProd->precio = temp->precio;
				oProd->codigo = temp->codigo;
				oProd->marca.append(temp->marca);
				oProd->desc.append(temp->desc);
				strcpy(oProd->foto1, temp->foto1);
				strcpy(oProd->foto2, temp->foto2);
				oProd->prev = NULL;
				oProd->next = NULL;
				aProd = oProd;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aProd->next != NULL)
					aProd = aProd->next;
				Productos* temp = new Productos;
				aProd->next = new Productos;
				lectorEscritor.seekg(i * sizeof(Productos));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Productos));
				aProd->next->prev = aProd;
				aProd = aProd->next;
				aProd->IDUser = temp->IDUser;
				aProd->IDProd = temp->IDProd;
				aProd->nombre.append(temp->nombre);
				aProd->inventario = temp->inventario;
				aProd->precio = temp->precio;
				aProd->codigo = temp->codigo;
				aProd->marca.append(temp->marca);
				aProd->desc.append(temp->desc);
				strcpy(aProd->foto1, temp->foto1);
				strcpy(aProd->foto2, temp->foto2);
				aProd->next = NULL;
				aProd = oProd;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		return;
	}
	else {
		return;
	}
}

void saveEnvio(Envios* pOrigen) {
	lectorEscritor.open("Envios.bin", ios::binary | ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		while (pOrigen != NULL) {
			lectorEscritor.write(reinterpret_cast<char*>(pOrigen), sizeof(Envios));
			pOrigen = pOrigen->next;
		}
		lectorEscritor.close();
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR", MB_ICONERROR);
		return;
	}
}

void loadEnvio() {
	lectorEscritor.open("Envios.bin", ios::binary | ios::in | ios::ate);
	if (lectorEscritor.is_open()) {
		int totalChar = lectorEscritor.tellg();
		if (totalChar < 1) {
			MessageBox(NULL, "Archivo vacio", "NO CARGAR", MB_ICONERROR);
			return;
		}
		for (int i = 0; i < totalChar / sizeof(Envios); i++) {
			if (oEnvio == NULL) {
				Envios* temp = new Envios;
				oEnvio = new Envios;
				lectorEscritor.seekg(i * sizeof(Envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));
				oEnvio->IDUser = temp->IDUser;
				oEnvio->IDProd = temp->IDProd;
				oEnvio->IDEnvio = temp->IDEnvio;
				oEnvio->cantidad = temp->cantidad;
				oEnvio->calle.append(temp->calle);
				oEnvio->colonia.append(temp->colonia);
				oEnvio->ciudad.append(temp->ciudad);
				oEnvio->estado = temp->estado;
				oEnvio->mensaje.append(temp->mensaje);
				oEnvio->fecha.append(temp->fecha);
				oEnvio->prev = NULL;
				oEnvio->next = NULL;
				aEnvio = oEnvio;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			else {
				while (aEnvio->next != NULL)
				aEnvio = aEnvio->next;
				Envios* temp = new Envios;
				aEnvio->next = new Envios;
				lectorEscritor.seekg(i * sizeof(Envios));
				lectorEscritor.read(reinterpret_cast<char*>(temp), sizeof(Envios));
				aEnvio->next->prev = aEnvio;
				aEnvio = aEnvio->next;
				aEnvio->IDUser = temp->IDUser;
				aEnvio->IDProd = temp->IDProd;
				aEnvio->IDEnvio = temp->IDEnvio;
				aEnvio->cantidad = temp->cantidad;
				aEnvio->calle.append(temp->calle);
				aEnvio->colonia.append(temp->colonia);
				aEnvio->ciudad.append(temp->ciudad);
				aEnvio->estado = temp->estado;
				aEnvio->mensaje.append(temp->mensaje);
				aEnvio->fecha.append(temp->fecha);
				aEnvio->cantidad = temp->cantidad;
				aEnvio->next = NULL;
				aEnvio = oEnvio;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
		}
		lectorEscritor.close();
		return;
	}
	else {
		return;
	}
}

void saveGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::out | ios::trunc);
	if (lectorEscritor.is_open()) {
		lectorEscritor << GLOBAL_USER_ID << endl;
		lectorEscritor << GLOBAL_PRODUCTO_ID << endl;
		lectorEscritor << GLOBAL_ENVIO_ID;
		
		lectorEscritor.close();
		return;
	}
	else {
		MessageBox(NULL, "No se pudo abrir el archivo", "NO GUARDAR ID", MB_ICONERROR);
		return;
	}
}

void loadGlobalId() {
	lectorEscritor.open("GlobalId.txt", ios::in);
	if (lectorEscritor.is_open()) {
		lectorEscritor >> GLOBAL_USER_ID;
		lectorEscritor >> GLOBAL_PRODUCTO_ID;
		lectorEscritor >> GLOBAL_ENVIO_ID;
		lectorEscritor.close();
		return;
	}
	else {
		return;
	}
}

void freeMemory() {
	if (oUser != NULL) {
		oUser = NULL;
		while (aUser != NULL) {
			User* temp = aUser;
			aUser = aUser->next;
			delete temp;
		}
	}

	if (oInfo != NULL) {
		oInfo = NULL;
		while (aInfo != NULL) {
			InfoVendedor* temp = aInfo;
			aInfo = aInfo->next;
			delete temp;
		}
	}

	if (oProd != NULL) {
		oProd = NULL;
		while (aProd != NULL) {
			Productos* temp = aProd;
			aProd = aProd->next;
			delete temp;
		}
	}

	if (oEnvio != NULL) {
		oEnvio = NULL;
		while (aEnvio != NULL) {
			Envios* temp = aEnvio;
			aEnvio = aEnvio->next;
			delete temp;
		}
	}

	return;
}
#pragma endregion