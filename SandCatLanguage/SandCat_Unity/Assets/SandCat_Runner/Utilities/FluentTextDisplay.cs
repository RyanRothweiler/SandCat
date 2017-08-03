using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

[RequireComponent(typeof(Text))]
public class FluentTextDisplay : MonoBehaviour
{
	public StateValue stateVal;

	private Text display;

	void Start ()
	{
		display = this.GetComponent<Text>();
	}

	void Update ()
	{
		int fluentValue = (int)stateVal.GetValue();
		display.text = fluentValue.ToString();
	}
}