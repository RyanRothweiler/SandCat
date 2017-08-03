using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SSGridCell : MonoBehaviour
{

	public Text oxyDispOne;
	public Text energyDispOne;
	public Text metalDispOne;

	public Text oxyDispTwo;
	public Text energyDispTwo;
	public Text metalDispTwo;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		if (this.GetComponent<GridCell>()) {
			DisplayAction(oxyDispOne, "oxy", "AOneOxygen");
			DisplayAction(energyDispOne, "ene", "AOneEnergy");
			DisplayAction(metalDispOne, "met", "AOneMetal");

			DisplayAction(oxyDispTwo, "oxy", "ATwoOxygen");
			DisplayAction(energyDispTwo, "ene", "ATwoEnergy");
			DisplayAction(metalDispTwo, "met", "ATwoMetal");
		}
	}

	public void DisplayAction(Text disp, string actionName, string fluentName)
	{
		int index = ((this.GetComponent<GridCell>().x - 1) * SandCat.instance.GetArrayHeight("Board")) + (this.GetComponent<GridCell>().y - 1) + 1;
		int val = (int)SandCat.instance.GetFluentInArray("Board", index, fluentName);
		disp.text = actionName + "-" + val;
	}
}
