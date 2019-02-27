#include "stdafx.h"
#include "LifeGameSystem.h"
#include <future>

// プロジェクトの関係上、メンバに置けないため無名名前空間で管理する
namespace {
	std::thread MainThread;
}

namespace LifeGameSystem {

	void LifeGameSystem::SetCellLive(int clickPosX, int clickPosY, bool isLive) const {

		int cellPosX, cellPosY;
		// マウスの位置にあるセルを取得
		_renderer->GetCellPosition(clickPosX, clickPosY, cellPosX, cellPosY);

		// セルを設定
		_cellManager->SetCellLive(cellPosX, cellPosY, isLive);
	}


	void LifeGameSystem::Start(int milliSec) {

		if (_isStarted) return;
		if (!_canStartLoop) return;

		_isStarted = true;

		// メインループが多重起動できないようにロックする
		_canStartLoop = false;

		// 別スレッド実行
		MainThread = thread(&LifeGameSystem::LoopAsync, this, milliSec, [this]() {
			// メインループのロック解除
			_canStartLoop = true;

			if (_onStoppedLoopOnce) {
				//実行したら即座に初期化
				_onStoppedLoopOnce();
				_onStoppedLoopOnce = nullptr;
			}
		

		});
		MainThread.detach();

	}

	void LifeGameSystem::LoopAsync(int milliSec, function<void()> callBack) {

		while (_isStarted) {

			// 止めるまで世代を更新し続ける
			Next();
			this_thread::sleep_for(chrono::milliseconds(milliSec));
		}

		callBack();

	}


}

