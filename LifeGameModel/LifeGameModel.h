#pragma once
#include "LifeGameSystem.h"

namespace LifeGameSystem {


	using namespace System;

	// LifeGameSystemにアクセスするための
	// ラッパークラス
	public ref class LifeGameModel {

		LifeGameSystem* _lifeGameSystem;	// ライフゲームを制御するクラス

	public:
		LifeGameModel(IntPtr windowHandle, int mapSizeWidth, int mapSizeHeight) :
			_lifeGameSystem(new LifeGameSystem(reinterpret_cast<HWND>(windowHandle.ToPointer()), mapSizeWidth, mapSizeHeight)) {}

		!LifeGameModel() { this->~LifeGameModel(); }
		~LifeGameModel() { delete _lifeGameSystem; }

		// 現在の世代数を取得
		const auto GetGeneration() {
			return _lifeGameSystem->GetGeneration();
		}

		// バックバッファを取得する
		const IntPtr GetBackBuffer() {
			return IntPtr(const_cast<void*>(_lifeGameSystem->GetBackBuffer()));
		}

		// マウスの座標から、セルの状態を変更する
		void SetCellLive(int clickPosX, int clickPosY, bool isLive) {
			_lifeGameSystem->SetCellLive(clickPosX, clickPosY, isLive);
		}

		// セルを初期化する
		void ClearCell() { _lifeGameSystem->ClearCell(); }

		// 指定した待ち時間でライフゲームの処理を開始する
		void Start(int milliSec) { _lifeGameSystem->Start(milliSec); }

		// ライフゲームの処理を止める
		void Stop() { _lifeGameSystem->Stop(); }

		// ライフゲームの処理を止めて再度開始する
		void ReStart(int milliSec) { _lifeGameSystem->ReStart(milliSec); }

		// 世代を1つ進める
		void Next() { _lifeGameSystem->Next(); }

		// 描画する
		void Draw() { _lifeGameSystem->Draw(); }

	};

}
