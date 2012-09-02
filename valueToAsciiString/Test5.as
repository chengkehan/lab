package
{
	import flash.display.Sprite;
	import flash.utils.getTimer;
	
	public class Test5 extends Sprite
	{
		private var _hexStr:Array = null;
		
		private var _jinzhi:int = 0;
		
		public function Test5()
		{
			_hexStr = new Array();
			for (var i:int = 0; i < 126; i++) 
			{
				_hexStr.push(String.fromCharCode(i));
			}
//			_hexStr = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];
//			trace(_hexStr);
			_jinzhi = _hexStr.length;
			
			var time:int = getTimer();
//			for (var j:int = 0; j <= 15875; j++) 
//			{
//				trace(tenToHex(j), tenToHex(j).length);
//			}
			var date:Number = 19999998;
			trace(tenToMyDecimal(date), tenToMyDecimal(date).length, date);
			trace(getTimer() - time);
		}
		
		public function tenToMyDecimal(input:Number):String
		{
			var o:String = "";
			while(input >= _jinzhi)
			{
				var newInput:Number = Math.floor(input / _jinzhi);
				o = _hexStr[input - newInput * _jinzhi] + o;
				input = newInput;
			}
			o = _hexStr[input] + o;
			
			return o;
		}
	}
}