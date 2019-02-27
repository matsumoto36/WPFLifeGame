#pragma once

#include <Windows.h>
#include <string>
#include <functional>

#include "Renderer.h"
#include "CellManager.h"

namespace LifeGameSystem {

	using namespace std;

	// ライフゲームで行う処理の橋渡しをする
	class LifeGameSystem {

		unique_ptr<Renderer> _renderer;			// 描画担当
		unique_ptr<CellManager> _cellManager;	// セルの管理担当

		unsigned int _generation;				// 進んだ世代
		bool _isStarted;						// 世代を更新しているか
		bool _canStartLoop;						// メインループが開始できるか

		function<void()> _onStoppedLoopOnce;	// ループが完全に止まった後に実行される。実行後は強制的にnullになる

	public:
		LifeGameSystem(HWND hwnd, int mapSizeWidth, int mapSizeHeight) :
			_renderer(new Renderer(hwnd)),
			_cellManager(new CellManager(mapSizeWidth, mapSizeHeight)),
			_generation(0),
			_isStarted(false),
			_canStartLoop(true),
			_onStoppedLoopOnce(nullptr) {

		}

		// 現在の世代数を取得
		const auto GetGeneration() const {
			return _generation;
		}

		// バックバッファの取得
		const auto GetBackBuffer() const {
			return _renderer->GetBackBuffer();
		}

		// マウスの座標から、セルの状態を変更する
		void SetCellLive(int clickPosX, int clickPosY, bool isLive) const;

		// セルを初期化する
		void ClearCell() {
			_generation = 0;
			_cellManager->Clear();
		}

		// 指定した待ち時間でライフゲームの処理を開始する
		void Start(int);

		// ライフゲームの処理を止める
		bool Stop() {
			if (_canStartLoop) return false;

			_generation = 0;
			_isStarted = false;

			return true;
		}

		// ライフゲームの処理を止める
		bool Stop(function<void()> onCompleted) {
			if (!Stop()) return false;
			_onStoppedLoopOnce = onCompleted;
			return true;
		}

		// ライフゲームの処理を止めて再度開始する
		void ReStart(int milliSec) {
			if (_canStartLoop) {
				// 止める処理が必要ない場合はそのまま開始
				Start(milliSec);
			}
			else {
				// 止めた後にリスタートする
				Stop([this, milliSec]() {
					Start(milliSec);
				});
			}
		}

		// 世代を1つ進める
		void Next() {
			_cellManager->GenerationUpdate();
			_generation++;
		}

		// 描画を行う
		void Draw() const {
			_renderer->Draw(*_cellManager);
		}

	private:

		// メインループ
		// 別スレッドで扱うことを想定
		void LoopAsync(int, function<void()> callBack);
	};

}