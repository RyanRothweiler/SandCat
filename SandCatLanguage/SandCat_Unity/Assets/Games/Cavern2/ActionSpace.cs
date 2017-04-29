using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ActionSpace : MonoBehaviour
{

	public string entityName;

	public Text nameDisplay;
	public Text currentDisplay;
	public Text additionDisplay;
	public GameObject workerDisplay;

	// Use this for initialization
	void Start ()
	{

	}

	// Update is called once per frame
	void Update ()
	{
		nameDisplay.text = entityName;
		currentDisplay.text = SandCat.instance.GetEntityFluentValue(entityName, "Current").ToString();
		additionDisplay.text = SandCat.instance.GetEntityFluentValue(entityName, "Addition").ToString();

		if (SandCat.instance.GetEntityFluentValue(entityName, "SpaceTaken") == 1) {
			workerDisplay.SetActive(true);
		} else {
			workerDisplay.SetActive(false);
		}
	}
}
