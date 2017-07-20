using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Character : MonoBehaviour
{

	public Grid board;
	public int arrayIndex;

	public Text atkText;
	public Text defText;
	public Text hpText;
	public Text goldText;
	public Text speedText;

	void Update ()
	{
		string arrayName = "Characters";

		int x = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "X");
		int y = (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Y");
		this.transform.position = board.GridToWorldPos(new Vector2(x, y));

		// atkText.text = "atk " + (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Atk");
		// atkText.text = "atk " + (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Atk");
		hpText.text = "HP " + (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "HP");
		goldText.text = "gold " + (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "GoldHeld");
		speedText.text = "speed " + (int)SandCat.instance.GetFluentInArray(arrayName, arrayIndex, "Speed");
	}
}