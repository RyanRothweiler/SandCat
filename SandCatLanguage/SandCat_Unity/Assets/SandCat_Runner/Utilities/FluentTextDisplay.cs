using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum FluentDisplayType {
	Fluent, Entity
}

public enum Comparison {
	AlwaysShow, LessThan, LessThanOrEqualTo, EqualTo, NotEqualTo, GreaterThan, GreaterThanOrEqualTo,
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
		float fluentValue = 0;
		if (type == FluentDisplayType.Fluent) {
			fluentValue = SandCat.instance.GetFluentValue(fluentName);
		} else if (type == FluentDisplayType.Entity) {
			fluentValue = SandCat.instance.GetEntityFluentValue(entityName, fluentName);
		}

		if (hideComparison == Comparison.AlwaysShow) {
			display.text = fluentValue.ToString();
		} else if (hideComparison == Comparison.LessThan) {
			DisplayOrHide(fluentValue < comparisonValue, fluentValue);
		} else if (hideComparison == Comparison.LessThanOrEqualTo) {
			DisplayOrHide(fluentValue <= comparisonValue, fluentValue);
		} else if (hideComparison == Comparison.EqualTo) {
			DisplayOrHide(fluentValue == comparisonValue, fluentValue);
		} else if (hideComparison == Comparison.NotEqualTo) {
			DisplayOrHide(fluentValue != comparisonValue, fluentValue);
		} else if (hideComparison == Comparison.GreaterThanOrEqualTo) {
			DisplayOrHide(fluentValue >= comparisonValue, fluentValue);
		} else if (hideComparison == Comparison.GreaterThan) {
			DisplayOrHide(fluentValue > comparisonValue, fluentValue);
		}
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