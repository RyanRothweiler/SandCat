using UnityEngine;
using UnityEditor;
[CustomEditor(typeof(FluentTextDisplay))]
public class FluentDisplayEditor : Editor
{
	public override void OnInspectorGUI()
	{
		// FluentTextDisplay fluentDisp = (FluentTextDisplay)target;

		// fluentDisp.type = (FluentDisplayType)EditorGUILayout.EnumPopup("Fluent Type", fluentDisp.type);

		// if (fluentDisp.type == FluentDisplayType.Entity) {
		// 	fluentDisp.entityName = EditorGUILayout.TextField("Entity Name", fluentDisp.entityName);
		// }
		// fluentDisp.fluentName = EditorGUILayout.TextField("Fluent Name", fluentDisp.fluentName);

		// fluentDisp.hideComparison = (Comparison)EditorGUILayout.EnumPopup("Hide When Comparison", fluentDisp.hideComparison);
		// if (fluentDisp.hideComparison != Comparison.AlwaysShow) {
		// 	fluentDisp.comparisonValue = EditorGUILayout.IntField("comparison Value", fluentDisp.comparisonValue);
		// }

		DrawDefaultInspector();
	}
}