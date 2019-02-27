#include "stdafx.h"
#include "Renderer.h"
#include "DxLib.h"

namespace LifeGameSystem {

	using namespace DxLib;

	Renderer::Renderer(HWND hwnd) : 
		_deadCellColor(GetColor(0, 0, 0)),		//黒
		_liveCellColor(GetColor(0, 255, 0)) {	//緑

		// Dxライブラリ初期化設定
		SetUseDirect3DVersion(DX_DIRECT3D_9EX);				// DirectX 9EXにする
		SetUserWindow(hwnd);								// 描画ウィンドウの設定
		ChangeWindowMode(TRUE);								// ウィンドウモードに設定
		SetWindowSizeChangeEnableFlag(TRUE);				// ウィンドウサイズを可変に

		GetDefaultState(&_viewWidth, &_viewHeight, NULL);	//ウィンドウサイズを取得
		SetGraphMode(_viewWidth, _viewHeight, 32);			// グラフィックモードの設定(最大)

		SetAlwaysRunFlag(TRUE);								// 非アクティブ時も処理続行
		SetDrawScreen(DX_SCREEN_BACK);						// 描画先をバックバッファへ設定
		SetUseFPUPreserveFlag(TRUE);						// FPUの精度を落とさない
		SetWaitVSyncFlag(FALSE);							// VSync同期を無効
		SetOutApplicationLogValidFlag(FALSE);				// ログ出力停止
		SetDoubleStartValidFlag(TRUE);						// 多重起動を許可
		SetUseIMEFlag(TRUE);								// IMEを有効

		// Dxライブラリの初期化
		if (DxLib_Init() == -1) return;

		// 背景色の設定
		SetBackgroundColor(32, 32, 32);

		// セルのサイズを設定
		SetCellSize(16);

	}

	Renderer::~Renderer() {
		// Dxライブラリの終了処理
		DxLib_End();
	}

	const void* Renderer::GetBackBuffer() const {
		// D3DImageに描画するために
		// バックバッファへのポインタを取得できるようにする
		return GetUseDirect3D9BackBufferSurface();
	}

	void Renderer::SetCellSize(int cellSize) {

		_cellPixelSize = cellSize;
		//1未満の場合は描画されないので、ボーダーレスになる
		_borderPixelSize = BorderScale * cellSize;

		//描画するセルの事前計算
		_cellAreaPixelSize = _cellPixelSize + _borderPixelSize;
		_drawCellCountWidth = _viewWidth / _cellAreaPixelSize + 1;
		_drawCellCountHeight = _viewHeight / _cellAreaPixelSize + 1;
	}

	void Renderer::Draw(CellManager& cellManager) const {

		//画面を消す
		ClearDrawScreen();

		// セルを描画
		for (int i = 0; i < _drawCellCountHeight; i++) {
			for (int j = 0; j < _drawCellCountWidth; j++) {

				auto startX = j * _cellAreaPixelSize;
				auto startY = i * _cellAreaPixelSize;

				auto color = cellManager.GetCellLive(j, i) ? _liveCellColor : _deadCellColor;

				DrawBox(startX, startY, startX + _cellPixelSize, startY + _cellPixelSize, color, true);
			}
		}

		ScreenFlip();
	}

}