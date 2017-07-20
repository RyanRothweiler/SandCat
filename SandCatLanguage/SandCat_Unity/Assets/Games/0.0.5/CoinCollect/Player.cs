using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Player : MonoBehaviour
{

	public Grid board;

	public Text valueDisplay;

	void Update ()
	{
		int x = (int)SandCat.instance.GetFluentValue("PlayerX");
		int y = (int)SandCat.instance.GetFluentValue("PlayerY");
		this.transform.position = board.GridToWorldPos(new Vector2(x, y));

		valueDisplay.text = "" + SandCat.instance.GetFluentValue("PlayerValue");
	}
}
