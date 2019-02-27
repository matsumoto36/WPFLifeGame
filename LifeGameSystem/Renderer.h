#pragma once
#include <Windows.h>
#include <string>

#include "CellManager.h"

namespace LifeGameSystem {

	using namespace std;

	// セルの描画を行うクラス
	class Renderer final {

		const float BorderScale = 0.125f;	// セルの大きさに対する枠のサイズ比

		int _deadCellColor;					// セルがOffになっているときの色
		int _liveCellColor;					// セルがOnになっているときの色

		int _viewWidth, _viewHeight;		// 出力される画面のサイズ

		int _cellPixelSize;					// 1つのセルの描画サイズ
		int _borderPixelSize;				// 枠線の描画サイズ
		int _cellAreaPixelSize;				// セルと枠線を足したサイズ

		int _drawCellCountWidth;			// 描画するセルの個数 SetCellSizeで事前に計算される
		int _drawCellCountHeight;

	public:
		Renderer(HWND);
		~Renderer();

		// バックバッファの取得
		const void* GetBackBuffer() const;

		// マウスの位置からセルの座標を求める
		void GetCellPosition(int mousePosX, int mousePosY, int& cellPosX, int& cellPosY) {

			cellPosX = mousePosX / _cellAreaPixelSize;
			cellPosY = mousePosY / _cellAreaPixelSize;
		}

		// セルのサイズを設定
		void SetCellSize(int cellSize);

		// 描画
		void Draw(CellManager&) const;
	};

}