import * as vscode from 'vscode';

export default class FlowFormatter
    implements vscode.DocumentFormattingEditProvider
{
    private console: vscode.OutputChannel;

    constructor(console: vscode.OutputChannel) {
        this.console = console;
    }

    provideDocumentFormattingEdits(
        document: vscode.TextDocument,
        options: vscode.FormattingOptions,
        token: vscode.CancellationToken
    ): vscode.TextEdit[] {
        return [];
    }
}
