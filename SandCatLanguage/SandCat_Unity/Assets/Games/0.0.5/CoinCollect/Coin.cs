using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Coin : MonoBehaviour
{
	public Grid board;
	public int index;
	public Text valueDisplay;

	// Update is called once per frame
	void Update ()
	{
		if ((int)SandCat.instance.GetFluentInArray("Coins", index, "Alive") == 1) {
			int x = (int)SandCat.instance.GetFluentInArray("Coins", index, "X");
			int y = (int)SandCat.instance.GetFluentInArray("Coins", index, "Y");
			this.transform.position = board.GridToWorldPos(new Vector2(x, y));

			valueDisplay.text = "" + SandCat.instance.GetFluentInArray("Coins", index, "Value");
		} else {
			Destroy(this.gameObject);
		}
	}
}
