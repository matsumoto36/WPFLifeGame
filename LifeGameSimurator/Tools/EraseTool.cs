using System.Windows;
using LifeGameSystem;

namespace LifeGameSimulator.Tools {

	/// <summary>
	/// セルを消すツール
	/// </summary>
	class EraseTool : ITool {

		private bool _isClick;

		public void OnMouseDown(LifeGameModel lifeGameModel, Point mousePosition) {
			_isClick = true;
			lifeGameModel.SetCellLive((int)mousePosition.X, (int)mousePosition.Y, false);
		}

		public void OnMouseMove(LifeGameModel lifeGameModel, Point mousePosition) {
			if(!_isClick) return;
			lifeGameModel.SetCellLive((int)mousePosition.X, (int)mousePosition.Y, false);

		}

		public void OnMouseUp(LifeGameModel lifeGameModel, Point mousePosition) {
			_isClick = false;
		}
	}
}
