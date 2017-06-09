using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum FluentDisplayType {
	Fluent, Entity
}

[RequireComponent(typeof(Text))]
public class FluentTextDisplay : MonoBehaviour
{
	// TODO this should use the new fluent class
	public FluentDisplayType type;

	public string entityName;
	public string fluentName;

	private Text display;

	public Comparison hideComparison;
	public int comparisonValue;

	// Use this for initialization
	void Start ()
	{
		if (SandCat.instance == null) {
			Debug.LogError("FluentTextDisplay requires a cat to be present.");
		}
		display = this.GetComponent<Text>();
	}

	// Update is called once per frame
	void Update ()
	{
		int fluentValue = (int)SandCat.instance.GetFluentValue(fluentName);
		display.text = fluentValue.ToString();
		// DisplayOrHide(hideComparison.IsTrue(fluentValue, comparisonValue), fluentValue);
	}

	private void DisplayOrHide(bool state, float value)
	{
		if (state) {
			display.text = value.ToString();
		} else {
			display.text = "";
		}
	}

}